#include "player.h"

#include <pthread.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

const int Player::HIGH_CARD_ratio = 1303488;      //高牌
const int Player::ONE_PAIR_ratio = 1098240;       //一对
const int Player::TWO_PAIR_ratio = 123552;       //两对
const int Player::THREE_OF_A_KIND_ratio = 54912;//三条
const int Player::STRAIGHT_ratio = 9216;       //顺子
const int Player::FLUSH_ratio = 5148;          //同花
const int Player::FULL_HOUSE_ratio = 3744;     //葫芦
const int Player::FOUR_OF_A_KIND_ratio = 624; //四条
const int Player::STRAIGHT_FLUSH_ratio = 36; //同花顺
const int Player::ALL_CARDS_STYLE_ratio = 2598960;//所有牌型

Player::Player(const char *SerIp, int SerPt, const char *MyIp, int MyPt, int pid, char *name)
{
    _pid = pid;
    pro = new Protocol(SerIp, SerPt, MyIp, MyPt,pid);//生成协议处理
    stg = new Strategy();

    my_money = 0;   //我的金钱
    my_jetton = 0;  //我的筹码

    _name = name;

    current_game_process = GAME_STATE_UN_KNOW;
    current_match_time = 0;
}

Player::~Player()
{
    delete pro;
    delete stg;
}

void Player::start_Protocol()
{
    if(pthread_create(&thread_pro, 0, protocol, (void*)this)){

        cout << "Pthread_create() error" << endl;
        exit(-1);
    }
}


void Player::start_Strategy()
{
    if(pthread_create(&thread_stg, 0, strategy, (void*)this)){

        cout << "Pthread_create() error" << endl;
        exit(-1);
    }
}

void Player::start_PlayCard()
{
    //将协议与player关联
    pro->connect_with(this);
    //启动协议模块线程
    start_Protocol();
    //启动策略模块现场
    start_Strategy();

    //打牌的其他逻辑可置于此处……


    //阻塞线程继续执行,放在最后两行
    pthread_join(thread_pro,0);
    pthread_join(thread_stg,0);
}

void Player::inform_game_state(game_state sta)
{
    current_game_process = sta;
}

void Player::inform_match_again()
{
    current_match_time++;
}

Strategy *Player::get_strategy()
{
    return stg;
}

//返回经过策略计算的动作
player_action Player::get_my_action()
{
    //(1)弃牌 BEGIN_GAME_FOLD_TIMES 局，避免疯狗，同时
    //期间统计选手的打牌风格，统计疯狗和怕死鬼
    if(current_match_time < BEGIN_GAME_FOLD_TIMES){
        return fold;
    }

    //计算出这次需要押进去的最小赌注数
    int need_min_bet = stg->get_playerBet().get_now_neet_minbet();

    /*====================两张牌的情况====================*/
    if(current_game_process == HOLD_CARDS_MSG){

//        //(2)如果当前排名为4以外，且只剩一个疯狗，另外的都是怕死鬼
//        if(stg->get_seatInfo().get_people_num()){
//            //看自己的手牌,如果很有把握赢，则跟注
//        }
//        else
//            myAction = all_in;


    }
    /*====================五张牌的情况====================*/
    if(current_game_process == FLOP_MSG){

    }
    /*====================六张牌的情况====================*/
    if(current_game_process == TURN_MSG){

    }
    /*====================七张牌的情况====================*/
    if(current_game_process == RIVER_MSG){

    }

    return myAction;
}

int Player::get_my_bet()
{
    return my_bet;
}

void* protocol(void *arg)
{
    Player *p = (Player *)arg;
    p->pro->startTcp();
    p->pro->process_Msg(p->_name);
    return NULL;
}
void* strategy(void *arg)
{
    Player *p = (Player *)arg;
    p->stg->start_compute();
    return NULL;
}


