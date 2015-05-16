#include "player.h"


Player::Player(const char *SerIp, int SerPt, const char *MyIp, int MyPt, int pid)
{
    pro = new Protocol(SerIp, SerPt, MyIp, MyPt,pid);//生成协议处理
    stg = new Strategy();
    my_money = 0;   //我的金钱
    my_jetton = 0;  //我的筹码
}

Player::~Player()
{
    delete pro;
    delete stg;
}
