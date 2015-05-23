#ifndef PLAYER_H
#define PLAYER_H

#include "enviroment.h"
#include "strategy.h"
#include "protocol.h"

#include <pthread.h>
//线程执行函数
//分别用来启动protocol和strategy
void* protocol(void *arg);
void* strategy(void *arg);

class Player
{
public:
    Player(const char *SerIp, int SerPt,
           const char* MyIp, int MyPt,
           int pid, char *name="liuyang");
    ~Player();

friend void* protocol(void *arg);
friend void* strategy(void *arg);

    /*功能：
        启动策略计算器
      参数：

      返回值：
        空
    */
    void start_Strategy();

    /*功能：
        启动协议处理器
      参数：

      返回值：
        空
    */
    void start_Protocol();

    //功能：开始打牌
    void start_PlayCard();
    //获得player的策略实例
    Strategy *get_strategy();
    //返回玩家的动作
    player_action get_my_action();
    //返回玩家准备加的注
    int get_my_bet();

private:
    /*我的资产情况*/
    int my_money;   //我的金钱
    int my_jetton;  //我的筹码
    int my_bet;     //我的赌注

    player_action lPAction;   //上家的行动
    player_action myAction;   //我的行动

    Protocol *pro;   //代表协议通信
    Strategy *stg;   //代表策略

    char *_name;    //代表玩家名pname
    pthread_t thread_pro;
    pthread_t thread_stg;

};

#endif // PLAYER_H
