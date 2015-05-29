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
    my_raise = 0;   //raise发送的赌注
    _name = name;

    old_need_min_bet = 0;

    current_game_process = GAME_STATE_UN_KNOW;
    last_game_process = GAME_STATE_UN_KNOW;
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
    //(1)弃牌 CONF_BEGIN_GAME_FOLD_TIMES 局，避免疯狗，同时
    //期间统计选手的打牌风格，统计疯狗和怕死鬼
    if(current_match_time < CONF_BEGIN_GAME_FOLD_TIMES){        
        return fold;
    }

    //计算出这次需要押进去的最小赌注数
    int need_min_bet = stg->get_playerBet().get_now_neet_minbet();

    /*====================两张牌的情况====================*/
    if(current_game_process == HOLD_CARDS_MSG){

        //“我”能为这次出牌给出的最大赌注
        int my_loved_bet = 0;
        //代表小盲注的倍率
        int extend = 0;

        stg->get_holdCards().can_use_compute_info();//这个函数通过，证明计算完成以下info均可用

        bool o_double = stg->get_holdCards().info_double;      //是对子
        bool o_distance_5 = stg->get_holdCards().info_distance_5;  //距离是5以内
        bool o_same_color = stg->get_holdCards().info_same_color;  //同样的颜色
        int  o_big_card_num = stg->get_holdCards().info_big_card_num;//有大牌存在
        cout << current_match_time << "Hand,my_turn_blind:"<< my_turn_blind << endl;
        if(my_turn_blind < 0){//说明不是我的盲注期间
            if((o_big_card_num == 0) && !o_double){//两个小牌
                cout << current_match_time << "Hand, flod action " << endl;
                return fold;
            }
        }
        //牌型统计
        extend = extend + 2 + o_big_card_num;
        if(o_same_color){
            extend += 1;
        }
        if(o_distance_5){
            extend += 1;
        }
        if(o_double && (o_big_card_num > 0)){   //大对子
            extend += 4;
        }
        if(o_double && (o_big_card_num == 0)){  //小对子
            extend += 2;
        }

        my_loved_bet = extend * CONF_SMALL_BLIND;   //可为当前手牌出的最大赌注
        my_bet = my_loved_bet;

        if(need_min_bet > my_bet)

            return fold;
        else
            myAction = check;
    }
    /*====================五张牌的情况====================*/
    if(current_game_process == FLOP_MSG){
        int my_loved_bet5 = 0;
        int extend5 = 0;

        if(stg->STYLE_flopCards == stg->STYLE_5Cards){//牌型未升级
            if(stg->get_holdCards().info_double || stg->get_holdCards().info_big_card_num == 2){
            //如果有一对手牌，或者有两个大牌，则加一个大忙注
                extend5 += 1;
            }
            if(stg->get_holdCards().info_double && stg->get_holdCards().info_big_card_num == 2){
            //如果有一对手牌且是两个大牌，则再一个大忙注
                extend5 += 1;
            }
            if(!(stg->get_holdCards().info_double) && (stg->get_holdCards().info_big_card_num == 0)){//否则直接弃掉
            //如果没有一对，且没有一张大牌
                if(old_need_min_bet != need_min_bet)//如果需要加入新注则弃掉
                    return fold;
            }
        }else{//牌型有升级
            if(stg->get_holdCards().info_double || stg->get_holdCards().info_big_card_num == 2){
            //如果有一对手牌，或者有两个大牌，则加一个大忙注
                extend5 += 2;
            }
            if(stg->get_holdCards().info_double && stg->get_holdCards().info_big_card_num == 2){
            //如果有一对手牌且是两个大牌，则再一个大忙注
                extend5 += 2;
            }
        }
        //如果游戏状态有过变化,才计算。为了避免同一个状态中，my_bet一直累加
        if(current_game_process != last_game_process){
            my_loved_bet5 = my_bet + extend5 * CONF_BIG_BLIND;
            my_bet = my_loved_bet5;
        }
        //cout << "FLOP_MSG.my_loved_bet = " << my_bet << endl;


        //如果不是以同一状态第二次进入这个分支，则对其置0
        if(last_game_process != current_game_process)
            raise_count = CONF_RAISE_COUNT;

        if(stg->STYLE_5Cards > FLUSH){//如果牌型果真很好，则加注
            raise_count--;
            if(raise_count > 0){//加到CONF_RAISE_COUNT次，
                my_raise = 1;
                last_game_process = current_game_process;
                old_need_min_bet = need_min_bet;//记录上次需要压入的赌注
                return raise;
            }
            else{
                myAction = check;
            }
        }

        if(need_min_bet > my_bet)
            return fold;
        else
            myAction =  check;
    }
    /*====================六张牌的情况====================*/
    if(current_game_process == TURN_MSG){
        int my_loved_bet6 = 0;
        int extend6 = 0;

        if(stg->STYLE_turnCards == stg->STYLE_6Cards){//牌型未升级
            if(stg->get_holdCards().info_double || stg->get_holdCards().info_big_card_num == 2){
            //如果有一对手牌，或者有两个大牌，则加一个大忙注
                extend6 += 1;
            }
            if(stg->get_holdCards().info_double && stg->get_holdCards().info_big_card_num == 2){
            //如果有一对手牌且是两个大牌，则再一个大忙注
                extend6 += 1;
            }
        }else{//牌型有升级
            if(stg->STYLE_5Cards == stg->STYLE_6Cards){//5张到六张无升级，测试目前牌型
                if(stg->STYLE_6Cards == ONE_PAIR)
                    extend6 += 1;
                if(stg->STYLE_6Cards == TWO_PAIR)
                    extend6 += 4;
                if(stg->STYLE_6Cards == THREE_OF_A_KIND)
                    extend6 += 8;
                if(stg->STYLE_6Cards == STRAIGHT)
                    extend6 += 12;
                if(stg->STYLE_6Cards > STRAIGHT)
                    extend6 += 16;
            }else{//5张到六张有升级，则极好

                if(stg->STYLE_6Cards == TWO_PAIR)
                    extend6 += 4;
                if(stg->STYLE_6Cards == THREE_OF_A_KIND)
                    extend6 += 8;
                if(stg->STYLE_6Cards == STRAIGHT)
                    extend6 += 12;
                if(stg->STYLE_6Cards > STRAIGHT)
                    extend6 += 16;
            }
        }

        if(stg->STYLE_6Cards == HIGH_CARD){//如果六张牌还是高牌，则最好弃掉
            if(old_need_min_bet < need_min_bet){//如果有人加注
                return fold;
            }
        }

        //如果游戏状态有过变化,为了避免同一个状态中，my_bet一直累加
        if(current_game_process != last_game_process){
            my_loved_bet6 = my_bet + extend6 * CONF_BIG_BLIND;
            my_bet = my_loved_bet6;
        }

        if(need_min_bet > my_bet)
            return fold;
        else
            myAction =  check;
    }
    /*====================七张牌的情况====================*/
    if(current_game_process == RIVER_MSG){
        int my_loved_bet7 = 0;
        int extend7 = 0;

        if(stg->STYLE_7Cards == HIGH_CARD){//如果七张牌还是高牌，则最好弃掉;或者跟着别人混到揭牌比大小
            if(old_need_min_bet < need_min_bet){//如果有人加注
                return fold;
            }
        }

        if(stg->STYLE_6Cards == ONE_PAIR)
            extend7 += 1;
        if(stg->STYLE_6Cards == TWO_PAIR)
            extend7 += 4;
        if(stg->STYLE_6Cards == THREE_OF_A_KIND)
            extend7 += 8;
        if(stg->STYLE_6Cards == STRAIGHT)
            extend7 += 12;

        my_raise = 0;
        //这种情况下，就一种判别，跟上！
        if(stg->STYLE_7Cards >= FULL_HOUSE){
            my_raise += 100;
        }
        if(stg->STYLE_7Cards >= FOUR_OF_A_KIND){
            my_raise += 100;
        }
        if(stg->STYLE_7Cards >=STRAIGHT_FLUSH){
            my_raise += 200;
        }

        //如果不是以同一状态第二次进入这个分支，则对其置0
        if(last_game_process != current_game_process)
            raise_count = CONF_RAISE_COUNT;

        if(my_raise > 0 && raise_count > 0){
            raise_count--;
            last_game_process = current_game_process;
            old_need_min_bet = need_min_bet + my_raise;//记录上次需要压入的赌注
            return raise;
        }
        else{
            last_game_process = current_game_process;
            old_need_min_bet = need_min_bet;//记录上次需要压入的赌注
            return check;
        }
        //如果游戏状态有过变化,为了避免同一个状态中，my_bet一直累加
        if(current_game_process != last_game_process){
            my_loved_bet7 = my_bet + extend7 * CONF_BIG_BLIND;
            my_bet = my_loved_bet7;
        }

        if(need_min_bet > my_bet){
            if(old_need_min_bet < need_min_bet)
                return fold;
            else
                myAction = check;
        }
        else{
            myAction = check;
        }
    }

    last_game_process = current_game_process;
    old_need_min_bet = need_min_bet;//记录上次需要压入的赌注
    return myAction;
}

int Player::get_my_raise()
{
    return my_raise;
}

void Player::set_my_turn_blind(int val)
{
    my_turn_blind = val;
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
    //Player *p = (Player *)arg;
    //p->stg->start_compute();
    return NULL;
}


