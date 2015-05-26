#include "protocol.h"
#include "string.h"
#include "player.h"

#include <sys/types.h>  //socket()
#include <sys/socket.h> //socket(),bind()
#include <arpa/inet.h>  //Inet_pton(),htons()

//#include <linux/tcp.h>  //IPPROTO_TCP选项
#include <netinet/tcp.h>

#include <stdio.h>      //sprintf()
#include <stdlib.h>     //strcpy
#include <strings.h>    //bzero()
#include <string.h>     //strtok()
#include <errno.h>      //strerro()
#include <unistd.h>     //usleep()
#include <iostream>
using namespace std;

Protocol::Protocol()
{

}

Protocol::~Protocol()
{
    delete[] _serverIP;
    delete[] _myIP;
}

Protocol::Protocol(const char *serverIP, int serverPT, const char *myIP, int myPT, int pid)
{
    int len = strlen(serverIP);
    _serverIP = new char[len + 1];
    strcpy(_serverIP,serverIP);

    len = strlen(myIP);
    _myIP = new char[len + 1];
    strcpy(_myIP, myIP);

    _serverPT = serverPT;
    _myPT = myPT;
    _pid = pid;

    gameStop = false;
}

int Protocol::startTcp()
{
    const int on = 1;
    //打开本地套接字
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0))== -1){
       cout << "socket() error!" << endl;
       return(-1);
    }
    setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    setsockopt(sock_fd, IPPROTO_TCP, TCP_NODELAY, &on, sizeof(on));

    //绑定本地IP和端口
    struct sockaddr_in cliaddr;
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;  //设定客户端类型
    cliaddr.sin_port = htons(_myPT);//设定客户端端口
    inet_pton(AF_INET, _myIP, &cliaddr.sin_addr);//设定客户端IP

    if((bind(sock_fd, (struct sockaddr *)&cliaddr, sizeof(cliaddr))) == -1){
        cout << "TCP-bind() error!" << endl;
        return(-1);
    }

    //设定服务器信息
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;  //设定服务器类型
    servaddr.sin_port = htons(_serverPT);//设定服务器端口
    inet_pton(AF_INET, _serverIP, &servaddr.sin_addr);//设定服务器IP

    while((connect(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr))) == -1){
//        cout << "connect() error!" << endl;
//        cout << strerror(errno) << endl;
//        cout.flush();
        usleep(5000);
    }
    return sock_fd;
}

void Protocol::connect_with(Player *ply)
{
    this->player = ply;
}

void Protocol::process_Msg(char *name)
{
    ptos_reg_msg(name);//向服务器注册自己的 pid 和 pname
    int len = 0;
    int index = 0;
    char *pline = NULL;

    while(gameStop == false)
    {
        if ((gameStop == false) && (len = recv(sock_fd, bufRecv, 1000, 0)) != 0) {
            bufRecv[len]='\0';
            cout << "--------------------"<<endl;
            cout << bufRecv <<endl;
            cout << "--------------------"<<endl;

            index = 0;

            while((pline = read_line_msg(index)) != NULL){                
            //=====================进入消息处理阶段=====================
                if(strcmp("seat/ ",pline) == 0){
                    stat = SEAT_INFO_MSG;
                    player->inform_match_again();
                    cout << "seat_info-msg" << endl;
                }
                if(strcmp("blind/ ",pline) == 0){
                    stat = BLIND_MSG;
                    cout << "blind-msg" << endl;
                }
                if(strcmp("hold/ ",pline) == 0){
                    stat = HOLD_CARDS_MSG;
                    player->inform_game_state(HOLD_CARDS_MSG);//通知player到达手牌状态
                    cout << "hold-cards-msg" << endl;
                }
                if(strcmp("inquire/ ",pline) == 0){
                    stat = INQUIRE_MSG;
                    cout << "inquire-msg" << endl;
                }
                if(strcmp("flop/ ",pline) == 0){
                    stat = FLOP_MSG;
                    player->inform_game_state(FLOP_MSG);//通知player到达手牌状态
                    cout << "flop-msg" << endl;
                }
                if(strcmp("turn/ ",pline) == 0){
                    stat = TURN_MSG;
                    player->inform_game_state(TURN_MSG);//通知player到达手牌状态
                    cout << "turn-msg" << endl;
                }
                if(strcmp("river/ ",pline) == 0){
                    stat = RIVER_MSG;
                    player->inform_game_state(RIVER_MSG);//通知player到达手牌状态
                    cout << "river-msg" << endl;
                }
                if(strcmp("showdown/ ",pline) == 0){
                    stat = SHOWDOWN_MSG;

                    cout << "showdown-msg" << endl;
                }
                if(strcmp("pot-win/ ",pline) == 0){
                    stat = POT_WIN_MSG;
                    cout << "pot-win-msg" << endl;
                }
                if(strcmp("game-over ",pline) == 0){
                    stat = GAME_OVER_MSG;
                    cout << "game-over-msg" << endl;
                }

                switch(stat)
                {
                    case(SEAT_INFO_MSG):stop_seat_info_msg(player,index);break;
                    case(BLIND_MSG):stop_blind_msg(player, index);break;
                    case(HOLD_CARDS_MSG):stop_hold_cards_msg(player, index);break;
                    case(INQUIRE_MSG):stop_inquire_msg(player, index); ptos_action_msg(player);break;
                    case(FLOP_MSG):stop_flop_msg(player, index);break;
                    case(TURN_MSG):stop_turn_msg(player, index);break;
                    case(RIVER_MSG):stop_river_msg(player, index);break;
                    case(SHOWDOWN_MSG):stop_showdown_msg(player, index);break;
                    case(POT_WIN_MSG):stop_pot_win_msg(player, index);break;
                    case(GAME_OVER_MSG):stop_game_over_msg(0);break;
                }
            }
        }
    }
}

bool Protocol::ptos_reg_msg(char *pname)
{
    bzero(bufSend, sizeof(bufSend));
    sprintf(bufSend,"reg: %d %s \n", _pid, pname);
    if((send(sock_fd, bufSend, strlen(bufSend)+1,0)) == -1){
        cout << "Protocol::ptos_reg_msg() error!" << endl;
        return(false);
    }
    return true;
}


bool Protocol::stop_seat_info_msg(Player *p, int &index)
{
    int count = 0;      //标记第几个人
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行
    Seat_info *seat = &(p->get_strategy()->get_seatInfo());

    while((pline = read_line_msg(index)) != NULL){

        if(strcmp("seat/ ",pline) == 0)
            continue;
        if(strcmp("/seat ",pline) == 0)
            break;

        //分词处理
        word = strtok(pline," ");
        while(word){
            if(strcmp("button:",word) == 0){word = strtok(NULL," \n"); continue;}
            if(strcmp("small",word) == 0) {word = strtok(NULL," \n"); continue;}
            if(strcmp("blind:",word) == 0) {word = strtok(NULL," \n"); continue;}
            if(strcmp("big",word) == 0) {word = strtok(NULL," \n"); continue;}

            seat->set_pid(count,atoi(word));
            word = strtok(NULL," ");

            seat->set_jetton(count, atoi(word));
            word = strtok(NULL," ");

            seat->set_money(count, atoi(word));
            word = strtok(NULL," ");
            count ++;
        }
    }
    seat->set_people_num(count);//设置当前比赛的人数
    return true;
}


bool Protocol::stop_game_over_msg(Player *p)
{
    gameStop = true;
    close(sock_fd);
    player->get_strategy()->inform_gameover();//通知strategy游戏结束
    return true;
}


bool Protocol::stop_blind_msg(Player *p, int &index)
{
    int count = 0;      //标记第几个人
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行
    Player_bet *pb = &(p->get_strategy()->get_playerBet());

    while((pline = read_line_msg(index)) != NULL){

        if(strcmp("blind/ ",pline) == 0)
            continue;
        if(strcmp("/blind ",pline) == 0)
            break;

        //分词处理
        word = strtok(pline,": ");
        while(word){

            pb->set_pid(count,atoi(word));
            word = strtok(NULL," ");

            pb->set_bet(count, atoi(word));
            word = strtok(NULL," ");

            count ++;
        }
    }
    return true;
}


bool Protocol::stop_hold_cards_msg(Player *p, int &index)
{
    int count = 0;      //标记第几个人
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行
    Hold_cards *ph = &(p->get_strategy()->get_holdCards());

    while((pline = read_line_msg(index)) != NULL){

        if(strcmp("hold/ ",pline) == 0)
            continue;
        if(strcmp("/hold ",pline) == 0)
            break;

        //分词处理
        word = strtok(pline," ");
        while(word){

            ph->set_color(count, s2card_color(word));
            word = strtok(NULL," ");

            ph->set_point(count, s2card_point(word));
            word = strtok(NULL," ");

            count ++;
        }
    }
    player->get_strategy()->inform_hold();
    return true;
}


bool Protocol::stop_inquire_msg(Player *p, int &index)
{
    int count = 0;      //标记第几个人
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行

    Player_bet *pb = &(p->get_strategy()->get_playerBet());
    Seat_info *pi = &(p->get_strategy()->get_seatInfo());

    while(pline = read_line_msg(index)){

        if(strcmp("inquire/ ",pline) == 0)
            continue;
        if(strcmp("/inquire ",pline) == 0)
            break;
        if(strncmp("total pot: ", pline, 9) == 0){

            word = strtok(pline,":");
            word = strtok(NULL, " ");
            pb->set_totl_pot(atoi(word));
            continue;
        }

        //分词处理
        word = strtok(pline," ");
        while(word){
            int temp_id = atoi(word);
            pb->set_pid(count, temp_id);
            word = strtok(NULL," ");

            pi->set_jetton(count, atoi(word));
            word = strtok(NULL," ");

            pi->set_money(count,atoi(word));
            word = strtok(NULL," ");

            pb->set_bet(count, atoi(word));
            word = strtok(NULL," ");

            pb->set_action(count, s2player_action(word));
            word = strtok(NULL," ");
            count ++;
        }
        pb->set_player_count(count);//设置当前投注人数

        //令strategy计算出“我”当前需要出的赌注是多少
    }
    return true;
}


bool Protocol::ptos_action_msg(Player *p)
{        
    player_action pl_act = no_player_action;
    pl_act = p->get_my_action();

    bzero(bufSend, sizeof(bufSend));//清空发送缓存

    if( pl_act == call)//跟注
        sprintf(bufSend,"%s \n", "call");

    if( pl_act == check)//让牌
        sprintf(bufSend,"%s \n", "check");

    if( pl_act == fold)//弃牌
        sprintf(bufSend,"%s \n", "fold");

    if( pl_act == all_in)//全押
        sprintf(bufSend,"%s \n", "all_in");

    if( pl_act == raise)//加注
        sprintf(bufSend,"%s %d \n", "call", p->get_my_bet());

    if((send(sock_fd, bufSend, strlen(bufSend)+1,0)) == -1){
        cout << "Protocol::ptos_action_msg() error!" << endl;
        return(false);
    }
    return true;
}


bool Protocol::stop_flop_msg(Player *p, int &index)
{
    int count = 0;      //标记第张牌
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行

    Public_cards *pc = &(p->get_strategy()->get_publicCards());

    while(pline = read_line_msg(index)){

        if(strcmp("flop/ ",pline) == 0)
            continue;
        if(strcmp("/flop ",pline) == 0)
            break;

        //分词处理
        word = strtok(pline," ");
        while(word){

            pc->set_color(count, s2card_color(word));
            word = strtok(NULL," ");

            pc->set_point(count, s2card_point(word));
            word = strtok(NULL," ");

            count ++;
        }
    }
    player->get_strategy()->inform_flop();
    return true;
}


bool Protocol::stop_turn_msg(Player *p, int &index)
{
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行

    Public_cards *pc = &(p->get_strategy()->get_publicCards());

    while((pline = read_line_msg(index)) != NULL){

        if(strcmp("turn/ ",pline) == 0)
            continue;
        if(strcmp("/turn ",pline) == 0)
            break;

        //分词处理
        word = strtok(pline," ");
        while(word){

            pc->set_color(3, s2card_color(word));
            word = strtok(NULL," ");

            pc->set_point(3, s2card_point(word));
            word = strtok(NULL," ");

        }
    }
    player->get_strategy()->inform_turn();
    return true;
}


bool Protocol::stop_river_msg(Player *p, int &index)
{
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行

    Public_cards *pc = &(p->get_strategy()->get_publicCards());

    while((pline = read_line_msg(index)) != NULL){

        if(strcmp("river/ ",pline) == 0)
            continue;
        if(strcmp("/river ",pline) == 0)
            break;

        //分词处理
        word = strtok(pline," ");
        while(word){
            pc->set_color(4, s2card_color(word));
            word = strtok(NULL," ");

            pc->set_point(4, s2card_point(word));
            word = strtok(NULL," ");

        }
    }
    player->get_strategy()->inform_river();
    return true;
}


bool Protocol::stop_showdown_msg(Player *p, int &index)
{
//    char *word = NULL;  //记录单词
//    char * pline = NULL;//记录读取到的行

//    Showdown_result *pr = &(p->get_strategy()->get_showdownResult());

//    while((pline = read_line_msg(index)) != NULL){

//        if(strcmp("showdown/ ",pline) == 0)
//            continue;
//        if(strcmp("/showdown ",pline) == 0)
//            break;

//        //分词处理
//        word = strtok(pline," ");
//        while(word){

//            pc->set_color(4, s2card_color(word));
//            word = strtok(NULL," ");

//            pc->set_point(4, s2card_point(word));
//            word = strtok(NULL," ");

//        }
//    }
    return true;
}


bool Protocol::stop_pot_win_msg(Player *p, int &index)
{
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行

    Win_allocation *pa = &(p->get_strategy()->get_winAllocation());

    while((pline = read_line_msg(index)) != NULL){

        if(strcmp("pot-win/ ",pline) == 0)
            continue;
        if(strcmp("/pot-win ",pline) == 0)
            break;

        //分词处理
        word = strtok(pline," ");
//        while(word){

//            pc->set_color(4, s2card_color(word));
//            word = strtok(NULL," ");

//            pc->set_point(4, s2card_point(word));
//            word = strtok(NULL," ");

//        }
    }
    return true;
}


char *Protocol::read_line_msg(int &index) const
{
    int len = 0;
    const char *tep = &bufRecv[index];

    while(bufRecv[index] != '\0')
    {
        len ++;
        index++;

        if(bufRecv[index] == '\n'){
            char *p = lines;
            strncpy(p,tep,len);
            p[len] = '\0';
            index ++;    //把位置移到换行符的下一个字符
            return p;
        }
    }
    return NULL;
}

