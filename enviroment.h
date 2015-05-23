#ifndef ENVIROMENT_H
#define ENVIROMENT_H

//德州扑克基本术语
enum basic_term
{
    button = 0,         //庄家
    blind = 1,          //盲注
    small_blind = 2,    //小盲注
    big_blind = 3,      //大盲注
    jetton = 4,         //筹码
    money = 5,          //金币
    bet = 6,            //赌注
    color = 7,          //花色
    point = 8           //点数
};

//打牌者行为
enum player_action
{
    check = 9,          //让牌
    call = 10,           //跟注
    raise = 11,          //加注
    all_in = 12,         //全押
    fold = 13           //弃牌
};

//成牌类型
enum card_result
{
    HIGH_CARD = 18,      //高牌
    ONE_PAIR = 19,       //一对
    TWO_PAIR = 20,       //两对
    THREE_OF_A_KIND = 21,//三条
    STRAIGHT = 22,       //顺子
    FLUSH = 23,          //同花
    FULL_HOUSE = 24,     //葫芦
    FOUR_OF_A_KIND = 25, //四条
    STRAIGHT_FLUSH = 26  //同花顺
};

//牌花色类型
enum card_color
{
    SPADES = 14,         //黑桃
    HEARTS = 15,         //红桃
    CLUBS = 16,          //梅花
    DIAMONDS = 17       //方片
};

enum game_state
{
    REG_MSG = 18,
    SEAT_INFO_MSG = 19,
    GAME_OVER_MSG = 20,
    BLIND_MSG = 21,
    HOLD_CARDS_MSG = 22,
    INQUIRE_MSG = 23,
    ACTION_MSG = 24,
    FLOP_MSG = 25,
    TURN_MSG = 26,
    RIVER_MSG = 27,
    SHOWDOWN_MSG = 28,
    POT_WIN_MSG = 29
};

enum card_point
{
    TWO = 31,
    THREE = 32,
    FOUR = 33,
    FIVE = 34,
    SIX = 35,
    SEVEN = 36,
    EIGHT = 37,
    NINE = 38,
    TEN = 39,
    J = 40,
    Q = 41,
    K = 42,
    A = 43
};

//从字符串到 card_point 转换
card_point s2card_point(char*s);
//从字符串到 card_color 转换
card_color s2card_color(char*s);

/*座次信息类
0：为庄家
1：小盲注
2：大盲注
3-7：一般牌手
*/
class Seat_info
{
public:
    Seat_info():people_num(0){}
    int get_pid(int index){return pid[index];}
    int get_jetton(int index){return jetton[index];}
    int get_money(int index){return money[index];}

    void set_pid(int index, int val){pid[index] = val; people_num++;}
    void set_jetton(int index, int val){jetton[index] = val;}
    void set_money(int index, int val){money[index] = val;}
private:
    int pid[8];         //玩家ID
    int jetton[8];      //筹码数
    int money[8];       //金币数
    int people_num;     //指在做比赛的人数
};

/*手牌类*/
class Hold_cards
{
public:
    card_color get_color(int index){return color[index];}
    card_point get_point(int index){return point[index];}

    void set_color(int index, card_color val){color[index] = val;}
    void set_point(int index, card_point val){point[index] = val;}
private:
    card_color color[2];        //牌的花色
    card_point point[2];        //牌的点数
};

/*玩家下注状态类*/
class Player_bet
{
public:
    Player_bet():totl_pot(0){}

    int get_pid(int index){return pid[index];}
    int get_bet(int index){return bet[index];}
    player_action get_action(int index){return action[index];}
    int get_totl_pot(){return totl_pot;}

    void set_pid(int index, int val){pid[index] = val;}
    void set_bet(int index, int val){bet[index] = val; totl_pot += val;}
    void set_action(int index, player_action val){action[index] = val;}
private:
    int pid[8];                         //玩家ID
    int bet[8];                         //玩家赌注
    player_action action[8];            //玩家的动作
    int totl_pot;                       //奖池数额
};

/*公有牌*/
class Public_cards
{
public:
    //构造函数
    Public_cards();
    //添加卡牌,可能与get_card_num()有竞争发生
    void add_one_card(card_color color,card_point point);
    //获取所有卡牌花色
    const card_color* get_colors();
    //获取所有卡牌点数
    const card_point* get_points();
    //获取当前公共牌到数量
    int get_card_num();

private:
    card_color _color[5];        //花色
    card_point _point[5];        //点数
    int _card_num;               //代表收到的卡牌数
};

/*摊牌结果*/
class Showdown_result
{
private:
    int pid[8];                         //玩家ID
    card_result nut_hand[8];            //玩家结果牌型
};

/*彩池金额分配*/
class Win_allocation
{
private:
    int pid[8];         //玩家ID
    int win_num[8];     //玩家赢取的奖金数额

};

#endif // ENVIROMENT_H
