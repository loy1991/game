#include "player.h"

#include <pthread.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

Player::Player(const char *SerIp, int SerPt, const char *MyIp, int MyPt, int pid, char *name)
{
    pro = new Protocol(SerIp, SerPt, MyIp, MyPt,pid);//生成协议处理
    stg = new Strategy();
    my_money = 0;   //我的金钱
    my_jetton = 0;  //我的筹码

    _name = name;
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
    //启动协议模块线程
    start_Protocol();
    //启动策略模块现场
    start_Strategy();

    //打牌的其他逻辑可置于此处……


    //阻塞线程继续执行,放在最后两行
    pthread_join(thread_pro,0);
    pthread_join(thread_stg,0);
}

void* protocol(void *arg)
{
    Player *p = (Player *)arg;
    p->pro->startTcp();
    p->pro->startBroadCast();
    p->pro->process_Msg(p->_name);
    return NULL;
}
void* strategy(void *arg)
{
    Strategy *p = (Strategy *)arg;
    return NULL;
}


