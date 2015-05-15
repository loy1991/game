#ifndef PLAYER_H
#define PLAYER_H

#include "protocol.h"

class Player
{
public:
    Player();

private:
int my_money;   //我的金钱
int my_jetton;  //我的筹码
Protocol::player_action lPAction;   //上家的行动
Protocol::player_action myAction;   //我的行动
};

/*我的资产情况*/
#endif // PLAYER_H
