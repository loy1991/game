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
           int pid, char *name="huluwa-culu");
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

    //通知player游戏进行的状态
    void inform_game_state(game_state sta);

    //通知player现在新进行了局游戏
    void inform_match_again();

/*供协议使用的两个回调函数*/
    //返回玩家的动作以便协议发送
    player_action get_my_action();
    //返回玩家准备加的注
    int get_my_raise();

    //设置是否有盲注投入
    void set_my_turn_blind(int val);
    //获得player的策略实例
    Strategy *get_strategy();

private:
    /*我的资产情况*/
    int my_money;   //我的金钱
    int my_jetton;  //我的筹码

    player_action myAction;//我的行动
    int my_bet;     //此刻我愿意压进的赌注
    int my_raise;   //raise发送的赌注

    int old_need_min_bet;  //上一轮最小需要压进的赌注
    int raise_count;       //最多加几次注的限制

//    player_action lPAction;   //上家的行动
    int my_turn_blind;  //代表该“我”出了盲注，记录盲注金额，无盲注则为-1

    Protocol *pro;   //代表协议通信
    Strategy *stg;   //代表策略

    char *_name;    //代表玩家名pname
    int _pid;        //代表玩家的pid

    //两个线程号
    pthread_t thread_pro;
    pthread_t thread_stg;

    game_state current_game_process;//当前游戏进度
    game_state last_game_process;   //上一次游戏进度

    int current_match_time;//代表当前比赛的局数

public:

    //各种牌型占的比例
    const static int HIGH_CARD_ratio;      //高牌
    const static int ONE_PAIR_ratio;       //一对
    const static int TWO_PAIR_ratio;       //两对
    const static int THREE_OF_A_KIND_ratio;//三条
    const static int STRAIGHT_ratio;       //顺子
    const static int FLUSH_ratio;          //同花
    const static int FULL_HOUSE_ratio;     //葫芦
    const static int FOUR_OF_A_KIND_ratio; //四条
    const static int STRAIGHT_FLUSH_ratio; //同花顺
    const static int ALL_CARDS_STYLE_ratio;//所有牌型
};

#endif // PLAYER_H
