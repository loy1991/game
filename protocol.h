#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <vector>
#include "strategy.h"
#include "player.h"

class Protocol
{
public:
    //德州扑克基本术语
    typedef enum
    {
        button = 0,         //庄家
        blind = 1,          //盲注
        small_blind = 2,    //小盲注
        big_blind = 3,      //大盲注
        jetton = 4,         //筹码
        money = 5,          //金币
        bet = 6,            //赌注
        color = 7,          //花色
        point = 8           //点数
    } basic_term;

    //打牌者行为
    typedef enum
    {
        check = 9,          //让牌
        call = 10,           //跟注
        raise = 11,          //加注
        all_in = 12,         //全押
        fold = 13           //弃牌
    } player_action;

    //成牌类型
    typedef enum
    {
        HIGH_CARD = 18,      //高牌
        ONE_PAIR = 19,       //一对
        TWO_PAIR = 20,       //两对
        THREE_OF_A_KIND = 21,//三条
        STRAIGHT = 22,       //顺子
        FLUSH = 23,          //同花
        FULL_HOUSE = 24,     //葫芦
        FOUR_OF_A_KIND = 25, //四条
        STRAIGHT_FLUSH = 26  //同花顺
    } card_result;

    //牌花色类型
    typedef enum
    {
        SPADES = 14,         //黑桃
        HEARTS = 15,         //红桃
        CLUBS = 16,          //梅花
        DIAMONDS = 17       //方片
    } card_color;

public:
    Protocol();
    /*功能：
        玩家向服务器注册消息
      参数：
          sock_fd:建立的套接字；
          pid：用户id
          pname：用户名
      返回值：
          注册成功则返回true，否则返回false
      */
    bool ptos_reg_msg(int sock_fd,int pid,char *pname);

    /*功能：
        服务器向玩家发送座位消息，将座次记录
      参数：
          seat：玩家座位对象
      返回值：
          设置成功则返回true，否则返回false
      */
    bool stop_seat_info_msg(Seat_info &seat);
    /*功能：
        服务器向玩家发游戏结束消息
      参数：
          空
      返回值：
          游戏结束则返回true，否则返回false
      */
    bool stop_game_over_msg(void);

    /*功能：
        服务器向玩家发盲注消息，只处理与本玩家ID有关的信息
      参数：
         player：代表“我"的玩家对象
      返回值：
          处理成功返回true，否则返回false
      */

    bool stop_blind_msg(Player &player);

    /*功能：
        服务器向玩家发手牌消息
      参数：
         holdCards：代表“我"的手牌
      返回值：
          处理成功返回true，否则返回false
      */
    bool stop_hold_cards_msg(Hold_cards &holdCards);

    /*功能：
        服务器向玩家发询问消息，一方面记录他人下注，另一方面，要向服务器发出“我”的动作
      参数：
         playerBet：代表其他人此次的下注情况
         player:代表玩家，通知玩家需要决策如何动作
      返回值：
          处理成功返回true，否则返回false
      */
    bool stop_inquire_msg(Player_bet &playerBet,Player &player);

    /*功能：
        玩家向服务器发送行动消息
      参数：
          player：代表玩家“我”
      返回值：
          执行成功则返回true，否则返回false
      */
    bool ptos_action_msg(int sock_fd, Player &player);

    /*功能：
        服务器向玩家发公牌消息
      参数：
         publicCards：代表公共的五张手牌
      返回值：
          处理成功返回true，否则返回false
      */
    bool stop_flop_msg(Public_cards &publicCards);

    /*功能：
        服务器向玩家发转牌消息
      参数：
         publicCards：代表公共的五张手牌
      返回值：
          处理成功返回true，否则返回false
      */
    bool stop_turn_msg(Public_cards &publicCards);

    /*功能：
        服务器向玩家发河牌消息
      参数：
         publicCards：代表公共的五张手牌
      返回值：
          处理成功返回true，否则返回false
      */
    bool stop_river_msg(Public_cards &publicCards);

    /*功能：
        服务器向玩家发摊牌消息
      参数：
         showdownResult：代表各个赌徒摊牌的结果
      返回值：
          处理成功返回true，否则返回false
      */
    bool stop_showdown_msg(Showdown_result &showdownResult);

    /*功能：
        服务器向玩家发彩池分配消息
      参数：
         player：代表"我"，相应的增减资产
         winAllocation：奖池分配，给策略统计
      返回值：
          处理成功返回true，否则返回false
      */
    bool stop_pot_win_msg(Player &player, Win_allocation &winAllocation);
};

#endif // PROTOCOL_H
