#include "enviroment.h"

#include <strings.h> //bzero()
#include <string.h>  //strcmp()
#include <stdlib.h>  //atoi()

#include <iostream>
using namespace std;

/*==============公有牌==============*/
Public_cards::Public_cards()
{
    bzero(_point, sizeof(_point));
    bzero(_color, sizeof(_color));
    _card_num = 0;
}

int Public_cards::get_card_num()
{
    return _card_num;
}

/*==============从字符串到 card_point 转换==============*/
card_point s2card_point(char*s)
{
    //J|Q|K|A
    if(strcmp("J",s) == 0) return J;
    if(strcmp("Q",s) == 0) return Q;
    if(strcmp("K",s) == 0) return K;
    if(strcmp("A",s) == 0) return A;
    int num = atoi(s);
//    if(num > 10){
//        cout << "wrong card_point" << endl;
//        return -1;
//    }
    switch(num){
    case(2):return TWO;
    case(3):return THREE;
    case(4):return FOUR;
    case(5):return FIVE;
    case(6):return SIX;
    case(7):return SEVEN;
    case(8):return EIGHT;
    case(9):return NINE;
    case(10):return TEN;
    }
}

/*==============从字符串到 card_color 转换==============*/
card_color s2card_color(char*s)
{
    if(strcmp("SPADES",s) == 0) return SPADES;//黑桃
    if(strcmp("HEARTS",s) == 0) return HEARTS;//红桃
    if(strcmp("CLUBS",s) == 0) return CLUBS;//梅花
    if(strcmp("DIAMONDS",s) == 0) return DIAMONDS;//方片
//    else{
//        cout << "wrong card_color" << endl;
//        return -1;
//    }
}

/*==============从字符串到 player_action 转换==============*/
player_action s2player_action(char*s)
{
    if(strcmp("check",s) == 0) return check;    //让牌
    if(strcmp("call",s) == 0) return call;      //跟注
    if(strcmp("raise",s) == 0) return raise;    //加注
    if(strcmp("all_in",s) == 0) return all_in;  //全押
    if(strcmp("fold",s) == 0) return fold;      //弃牌
    else
        return no_player_action;                //无动作
}

/*==============玩家下注状态类==============*/
int Player_bet::get_index(int id)
{
    //设置策略中，“我”坐得位置
    for(int i = 0; i < player_count; i++){
        if(pid[i] == id){
            return i;//给strategy设置“我”坐得位置
        }
    }
    return -1;
}

int Player_bet::get_bet_of_pid(int id)
{
    for(int i = 0; i<player_count; i++){
        if(pid[i] == id){
            return bets[i];
        }
    }
    return -1;
}

int Player_bet::get_now_neet_minbet()
{
    for(int i = 0; i < player_count; i++){
        if(action[i] != fold)
            return bets[i];
    }
    return -1;
}

/*==============座次信息类==============*/
int Seat_info::get_index(int id)
{
    //设置策略中，“我”坐得位置
    for(int i = 0; i < 8; i++){
        if(pid[i] == id){
            return i;//给strategy设置“我”坐得位置
        }
    }
    return -1;
}


/*==============手牌类==============*/
void Hold_cards::same_color()
{
    if(_color[0] == _color[1])
        info_same_color = true;
}

void Hold_cards::distance_5()
{
    int dis = -1;
    dis = ((dis = (_point[0] - _point[1])) >= 0) ? dis:-dis;
    if(dis == 0)
        info_double = true;
    if(dis <= 5)
        info_distance_5 = true;
}

void Hold_cards::big_card()
{
    if(_point[0] > TEN)
        info_big_card_num++;
    if(_point[1] > TEN)
        info_big_card_num++;
}

void Hold_cards::compute_info()
{
    BL_Lock bl(&info_mutex);

    clean_info();
    same_color();
    distance_5();
    big_card();

//    cout << "d "<< info_double << "\t";      //是对子
//    cout << "d5 "<< info_distance_5 << "\t";  //距离是5以内
//    cout <<"s "<< info_same_color << "\t";  //同样的颜色
//    cout << "b "<< info_big_card_num << endl;//有大牌存在
}

void Hold_cards::clean_info()
{
    info_double = false;      //是对子
    info_distance_5 = false;  //距离是5以内
    info_same_color = false;  //同样的颜色
    info_big_card_num = 0;//有大牌存在
}

