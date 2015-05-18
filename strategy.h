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

protected:
    //高牌概率
    float high_card_chance();
    bool is_high_card();

    //一对概率
    float one_pair_chance();
    bool is_one_pair();

    //两对概率
    float two_pair_chance();
    bool is_two_pair();

    //三条概率
    float three_of_a_kind_chance();
    bool is_three_of_a_kind();

    //顺子概率
    float straight_chance();
    bool is_straight();

    //同花概率
    float flush_chance();
    bool is_flush();

    //葫芦概率
    float full_house_chance();
    bool is_full_house();

    //四条概率
    float four_of_a_kind_chance();
    bool is_four_of_a_kind();

    //同花顺概率
    float straight_flush_chance();
    bool is_straight_flush();


private:

    Win_allocation winAllocation;
    Showdown_result showdownResult;
    Public_cards publicCards;
    Player_bet playerBet;
    Hold_cards holdCards;
    Seat_info seatInfo;

    //为计算"成牌"方便,设置牌型数组,做记号
    int card_state[4][13];//card array

};

#endif // STRATEGY_H
