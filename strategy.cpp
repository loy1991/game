#include "strategy.h"

Strategy::Strategy()
{
}

card_result Strategy::what_card_result()
{
    //if(publicCards.get_card_num() >=3)
}

//高牌概率
float Strategy::high_card_chance()
{

}
bool Strategy::is_high_card()
{

}

//一对概率
float Strategy::one_pair_chance()
{

}
bool Strategy::is_one_pair()
{

}

//两对概率
float Strategy::two_pair_chance()
{

}
bool Strategy::is_two_pair()
{

}

//三条概率
float Strategy::three_of_a_kind_chance()
{

}
bool Strategy::is_three_of_a_kind()
{

}

//顺子概率
float Strategy::straight_chance()
{

}
bool Strategy::is_straight()
{

}

//同花概率
float Strategy::flush_chance()
{

}
bool Strategy::is_flush()
{

}

//葫芦概率
float Strategy::full_house_chance()
{

}
bool Strategy::is_full_house()
{

}

//四条概率
float Strategy::four_of_a_kind_chance()
{

}
bool Strategy::is_four_of_a_kind()
{

}

//同花顺概率
float Strategy::straight_flush_chance()
{

}
bool Strategy::is_straight_flush()
{
    for(int i = 0; i < 4; i++){

    }
}

int Strategy::max_succession(const char *arry, int len, int &index) const
{
    int max_len = -1;
    int max_len_index = -1;
    int temp;

    for(int i = 0; i < len;i++){
        temp = 0;
        while(arry[i]){
            temp++;
            i++;
        }
        if(temp > max_len){
            max_len = temp;
            max_len_index = i - temp;
        }
    }

    index = max_len_index;
    return max_len;
}

