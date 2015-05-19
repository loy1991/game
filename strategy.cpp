#include "strategy.h"
#include "string.h"
Strategy::Strategy()
{
}

card_result Strategy::what_card_result()
{
    //if(publicCards.get_card_num() >=3)
}

//高牌概率,其实这个计算大可不必
float Strategy::high_card_chance()
{

}
bool Strategy::is_high_card()
{
    //没有一对，不是顺子，不是同花，就是高牌了
    if(!(is_one_pair()) && !(is_straight()) && !(is_flush()))
        return true;
    else
        return false;
}

//一对概率
float Strategy::one_pair_chance()
{

}
bool Strategy::is_one_pair() const
{
    int more_two = 0;   //大于两张点数相同的牌
    int count;          //用于统计

    for(int i = 0; i < 13; i++){
        count = 0;
        for(int j = 0; j < 4; j++){
            if(card_state[j][i] != 0){
                count++;
            }
        }
        if(count >= 2)
            more_two++;
    }
    if(more_two >= 1)
        return true;
    else
        return false;
}

//两对概率
float Strategy::two_pair_chance()
{

}
bool Strategy::is_two_pair() const
{
    int more_two = 0;   //大于两张点数相同的牌
    int count;          //用于统计

    for(int i = 0; i < 13; i++){
        count = 0;
        for(int j = 0; j < 4; j++){
            if(card_state[j][i] != 0){
                count++;
            }
        }
        if(count >= 2)
            more_two++;
    }
    if(more_two >= 2)
        return true;
    else
        return false;
}

//三条概率
float Strategy::three_of_a_kind_chance()
{

}
bool Strategy::is_three_of_a_kind() const
{
    int count;

    for(int i = 0; i < 13; i++){
        count = 0;
        for(int j = 0; j < 4; j++){
            if(card_state[j][i] != 0){
                count++;
            }
        }
        if(count == 3)
            return true;
    }
    return false;
}

//顺子概率
float Strategy::straight_chance()
{

}
bool Strategy::is_straight() const
{
    unsigned int state[13];
    int index = -1;
    memset(state,0,sizeof(state));
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 13; j++){
            if(card_state[i][j] != 0)
                state[j] = 1;
        }
    }
    if(max_succession(state,13,index) >= 5)
        return true;
    else
        return false;
}

//同花概率
float Strategy::flush_chance()
{

}
bool Strategy::is_flush() const
{
    int len;
    for(int i = 0; i < 4; i++){
        len = 0;
        for(int j = 0; j < 13; j++){
            if(card_state[i][j] != 0)
                len++;
        }
        if(len >= 5)
            return true;
    }
    return false;
}

//葫芦概率
float Strategy::full_house_chance()
{

}
bool Strategy::is_full_house() const
{
    int more_three = 0; //大于三张的相同点数的牌
    int more_two = 0;   //大于两张的相同点数的牌
    int count;          //用于统计

    for(int i = 0; i < 13; i++){
        count = 0;
        for(int j = 0; j < 4; j++){
            if(card_state[j][i] != 0){
                count++;
            }
        }
        if(count >= 2)
            more_two++;
        if(count >= 3)
            more_three++;
    }
    if(more_three >= 2 || (more_three == 1 && more_two > 1))
        return true;
    else
        return false;
}

//四条概率
float Strategy::four_of_a_kind_chance()
{

}
bool Strategy::is_four_of_a_kind()const
{
    int count;

    for(int i = 0; i < 13; i++){
        count = 0;
        for(int j = 0; j < 4; j++){
            if(card_state[j][i] != 0){
                count++;
            }
        }
        if(count == 4)
            return true;
    }
    return false;
}

//同花顺概率
float Strategy::straight_flush_chance()
{

}
bool Strategy::is_straight_flush()const
{
    int index = -1;
    for(int i = 0; i < 4; i++){
        if((max_succession(card_state[i], 13, index)) >= 5){
            return true;
        }
    }
    return false;
}

int Strategy::max_succession(const unsigned int *arry, int len, int &index) const
{
    int max_len = -1;
    int max_len_index = -1;
    int temp;

    for(int i = 0; i < len;i++){
        temp = 0;
        while(arry[i] && i < len){
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

