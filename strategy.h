#ifndef STRATEGY_H
#define STRATEGY_H

#include "protocol.h"

class Strategy
{
public:
    Strategy();

private:

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
    Protocol::player_action action[8];  //玩家的动作
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
    Protocol::card_result nut_hand[8];  //玩家结果牌型
};

/*彩池金额分配*/
class Win_allocation
{
private:
    int pid[8];         //玩家ID
    int win_num[8];     //玩家赢取的奖金数额
};
#endif // STRATEGY_H
