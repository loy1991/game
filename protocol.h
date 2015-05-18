#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <vector>
#include "enviroment.h"

class Player;//声明此类，头文件相互包含的处理

class Protocol
{
public:

    /*初始化客户端参数*/
    Protocol();
    Protocol(const char *serverIP, int serverPT, const char *myIP, int myPT, int pid);

    /*功能：
        启动客户端，与服务器建立TCP连接
      参数：
        空
      返回值：
          返回建立连接的套接字，否则-1
      */
    int startTcp();

    /*功能：
        启动广播套接字
      参数：
        空
      返回值：
          返回建立连接的套接字，否则-1
      */
    int startBroadCast();

    /*功能：
        将协议与player连系起来
      参数：
        ply
      返回值：
        空
      */
    void connect_with(Player *ply);

    /*功能：
        开始处理消息
      参数：
        name：注册的pname
      返回值：
        空
      */
    void process_Msg(char *name);
    /*功能：
        玩家向服务器注册消息
      参数：
          sock_fd:建立的套接字；
          pid：用户id
          pname：用户名
      返回值：
          注册成功则返回true，否则返回false
      */
    bool ptos_reg_msg(char *pname);

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
    bool stop_game_over_msg(void *);

    /*功能：
        服务器向玩家发盲注消息，只处理与本玩家ID有关的信息
      参数：
         player：代表“我"的玩家对象
      返回值：
          处理成功返回true，否则返回false
      */

    bool stop_blind_msg();

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

      返回值：
          处理成功返回true，否则返回false
      */
    bool stop_inquire_msg(Player_bet &playerBet);

    /*功能：
        玩家向服务器发送行动消息
      参数：
          player：代表玩家“我”
      返回值：
          执行成功则返回true，否则返回false
      */
    bool ptos_action_msg(int sock_fd);

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
    bool stop_pot_win_msg(Win_allocation &winAllocation);

private:
    Player *player; //代表玩家“我”

    //需要用户输入的注册信息
    char *_serverIP;
    int _serverPT;
    char *_myIP;
    int _myPT;
    int _pid;

    //通信的套接字
    int sock_fd;
    int broadcast_fd;

    //游戏结束标记
    bool gameStop;

    //消息接受与发送缓存
    char bufRecv[500];
    char bufSend[100];

};

#endif // PROTOCOL_H
