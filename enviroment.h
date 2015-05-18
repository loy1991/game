#ifndef ENVIROMENT_H
#define ENVIROMENT_H

//德州扑克基本术语
enum basic_term
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
};

//打牌者行为
enum player_action
{
    check = 9,          //让牌
    call = 10,           //跟注
    raise = 11,          //加注
    all_in = 12,         //全押
    fold = 13           //弃牌
};

//成牌类型
enum card_result
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
};

//牌花色类型
enum card_color
{
    SPADES = 14,         //黑桃
    HEARTS = 15,         //红桃
    CLUBS = 16,          //梅花
    DIAMONDS = 17       //方片
};
enum game_state
{
    REG_MSG = 18,
    SEAT_INFO_MSG = 19,
    GAME_OVER_MSG = 20,
    BLIND_MSG = 21,
    HOLD_CARDS_MSG = 22,
    INQUIRE_MSG = 23,
    ACTION_MSG = 24,
    FLOP_MSG = 25,
    TURN_MSG = 26,
    RIVER_MSG = 27,
    SHOWDOWN_MSG = 28,
    POT_WIN_MSG = 29
};

/*座次信息类
0：为庄家
1：小盲注
2：大盲注
3-7：一般牌手
*/
class Seat_info
{
private:
    int pid[8];         //玩家ID
    int jetton[8];      //筹码数
    int money[8];       //金币数
};

/*手牌类*/
class Hold_cards
{
private:
    unsigned char color[2];     //牌的花色
    unsigned char point[2];     //牌的点数
};

/*玩家下注状态类*/
class Player_bet
{
private:
    int pid[8];                         //玩家ID
    int money[8];                       //玩家金币
    int bet[8];                         //玩家赌注
    player_action action[8];            //玩家的动作
    int totl_pot;                       //奖池数额
};

/*公有牌*/
class Public_cards
{
private:
    unsigned char color[5];     //花色
    unsigned char point[5];     //点数
};

/*摊牌结果*/
class Showdown_result
{
private:
    int pid[8];                         //玩家ID
    card_result nut_hand[8];            //玩家结果牌型
};

/*彩池金额分配*/
class Win_allocation
{
private:
    int pid[8];         //玩家ID
    int win_num[8];     //玩家赢取的奖金数额
};

#endif // ENVIROMENT_H
