#ifndef STRATEGY_H
#define STRATEGY_H
#include "enviroment.h"

class Player;//声明此类，头文件相互包含的处理

class Strategy
{
public:
    Strategy();

    /*功能：
          供player询问此时“我”的成牌情况，三张以上的公牌时候可以查寻
      参数：
        空
      返回值：
          返回成牌的情况
    */
    card_result what_card_result();

private:

    Win_allocation winAllocation;
    Showdown_result showdownResult;
    Public_cards publicCards;
    Player_bet playerBet;
    Hold_cards holdCards;
    Seat_info seatInfo;
};

#endif // STRATEGY_H
