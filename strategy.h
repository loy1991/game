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

    //返回策略保存情报的单元
    Seat_info& get_seatInfo();
    Player_bet& get_playerBet();
    Hold_cards& get_holdCards();
    Public_cards& get_publicCards();
    Showdown_result& get_showdownResult();
    Win_allocation& get_winAllocation();

    //开始计算,用于线程函数处
    void start_compute();

    //通知strategy现在的游戏状态
    void inform_hold();
    void inform_flop();
    void inform_turn();
    void inform_river();
    void inform_gameover();


    //计算不同游戏进程中，牌型的变化
    void compute_STYLE_holdCards();
    void compute_STYLE_flopCards();
    void compute_STYLE_turnCards();
    void compute_STYLE_riverCards();

protected:
    //高牌概率
    float high_card_chance();
    inline bool is_high_card();

    //一对概率
    float one_pair_chance();
    inline bool is_one_pair() const;

    //两对概率
    float two_pair_chance();
    inline bool is_two_pair() const;

    //三条概率
    float three_of_a_kind_chance();
    inline bool is_three_of_a_kind() const;

    //顺子概率
    float straight_chance();
    inline bool is_straight() const;

    //同花概率
    float flush_chance();
    inline bool is_flush() const;

    //葫芦概率
    float full_house_chance();
    inline bool is_full_house() const;

    //四条概率
    float four_of_a_kind_chance();
    inline bool is_four_of_a_kind() const;

    //同花顺概率
    float straight_flush_chance();
    inline bool is_straight_flush() const;


private:
    /*功能：
          返回数组中最大的连续牌个数;值返回连续开始的位置
      参数：
        arry:待检查的数组
        index：值返回，返回最大连续牌的开始位置
        len：数组长度
      返回值：
          返回成牌的情况
    */
    int max_succession(const unsigned int *arry, int len, int &index) const;

    //清空card_state中的标记
    void clean_card_state();
    card_result largest_style();

private:
    Win_allocation winAllocation;
    Showdown_result showdownResult;
    Public_cards publicCards;
    Player_bet playerBet;
    Hold_cards holdCards;
    Seat_info seatInfo;

    //为计算"成牌"方便,设置牌型数组,做记号
    unsigned int card_state[4][13];//card array

    bool game_process_hold; //当前游戏进度,指示hold牌计算
    bool game_process_flop; //当前游戏进度,指示flop牌计算
    bool game_process_turn; //当前游戏进度,指示turn牌计算
    bool game_process_river;//当前游戏进度,指示river牌计算
    bool game_process_gameover;

public:
    //用来记录此次游戏进度下的牌型记录
    card_result STYLE_holdCards;
    card_result STYLE_flopCards;
    card_result STYLE_5Cards;
    card_result STYLE_turnCards;
    card_result STYLE_6Cards;
    card_result STYLE_riverCards;
    card_result STYLE_7Cards;
};

#endif // STRATEGY_H
