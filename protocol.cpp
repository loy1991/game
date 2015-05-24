#include "protocol.h"
#include "string.h"
#include "player.h"

#include <sys/types.h>  //socket()
#include <sys/socket.h> //socket(),bind()
#include <arpa/inet.h>  //Inet_pton(),htons()

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
    while(gameStop == false)
    {
        if ((len = recv(sock_fd, bufRecv, 500, 0)) != 0) {
            bufRecv[len]='\0';

            //获取数据的首行
            int index = 0;
            char * pline = read_line_msg(index);

            //进入消息处理阶段

            //game_over消息
            if(strcmp("game-over ",pline) == 0){
                delete pline;
                stop_game_over_msg(0);
            }
            //seat_info消息
            if(strcmp("seat/ ",pline) == 0){
                delete pline;
                stop_seat_info_msg(player->get_strategy()->get_seatInfo());
            }
            //blind-msg消息
            if(strcmp("blind/ ",pline) == 0){
                delete pline;
                stop_blind_msg(player);
            }
            //hold-cards-msg消息
            if(strcmp("hold/ ",pline) == 0){
                delete pline;
                stop_hold_cards_msg(player);
            }
            //inquire-msg消息|发送action-msg
            if(strcmp("inquire/ ",pline) == 0){
                delete pline;
                //Player_bet
                stop_inquire_msg(player);
                ptos_action_msg(player);
            }
            //flop-msg公牌
            if(strcmp("flop/ ",pline) == 0){
                delete pline;
                //Public_cards
                stop_flop_msg(player);
            }
            //turn-msg
            if(strcmp("turn/ ",pline) == 0){
                delete pline;
                //Public_cards
                stop_turn_msg(player);
                cout << "turn card" << endl;
            }
            //river-msg
            if(strcmp("river/ ",pline) == 0){
                delete pline;
                //Public_cards
                stop_river_msg(player);
                cout << "river card" << endl;
            }
            //showdown-msg
            if(strcmp("showdown/ ",pline) == 0){
                delete pline;
                //Showdown_result
                stop_showdown_msg(player);
                cout << "showdown" << endl;
            }
            //pot-win-msg
            if(strcmp("pot-win/ ",pline) == 0){
                delete pline;
                //Win_allocation
                stop_pot_win_msg(player);
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

bool Protocol::stop_seat_info_msg(Seat_info &seat)
{
    int index = 0;
    int count = 0;      //标记第几个人
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行

    while(pline = read_line_msg(index)){

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

            seat.set_pid(count,atoi(word));
            word = strtok(NULL," ");

            seat.set_jetton(count, atoi(word));
            word = strtok(NULL," ");

            seat.set_money(count, atoi(word));
            word = strtok(NULL," ");
            count ++;
        }
        delete pline;
    }
    return true;
}

bool Protocol::stop_game_over_msg(void*)
{
    gameStop = true;
    return true;
}

bool Protocol::stop_blind_msg(Player *p)
{
    int index = 0;
    int count = 0;      //标记第几个人
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行
    Player_bet *pb = &(p->get_strategy()->get_playerBet());

    while(pline = read_line_msg(index)){

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

        delete pline;
    }
    return true;
}

bool Protocol::stop_hold_cards_msg(Player *p)
{
    int index = 0;
    int count = 0;      //标记第几个人
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行
    Hold_cards *ph = &(p->get_strategy()->get_holdCards());

    while(pline = read_line_msg(index)){

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

        delete pline;
    }
    return true;
}

bool Protocol::stop_inquire_msg(Player *p)
{
    int index = 0;
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

            pi->set_jetton(pi->get_index(temp_id), atoi(word));
            word = strtok(NULL," ");

            pi->set_money(pi->get_index(temp_id),atoi(word));
            word = strtok(NULL," ");

            pb->set_bet(count, atoi(word));
            word = strtok(NULL," ");

            pb->set_action(count, s2player_action(word));
            word = strtok(NULL," ");
            count ++;
        }
        delete pline;
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


bool Protocol::stop_flop_msg(Player *p)
{
    int index = 0;
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
        delete pline;
    }
    return true;
}


bool Protocol::stop_turn_msg(Player *p)
{
    int index = 0;
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行

    Public_cards *pc = &(p->get_strategy()->get_publicCards());

    while(pline = read_line_msg(index)){

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
        delete pline;
    }
    return true;
}


bool Protocol::stop_river_msg(Player *p)
{
    int index = 0;
    char *word = NULL;  //记录单词
    char * pline = NULL;//记录读取到的行

    Public_cards *pc = &(p->get_strategy()->get_publicCards());

    while(pline = read_line_msg(index)){

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
        delete pline;
    }
    return true;
}


bool Protocol::stop_showdown_msg(Player *p)
{
    int index = 0;
    char * pline = read_line_msg(index);


    delete pline;
    return true;
}

bool Protocol::stop_pot_win_msg(Player *p)
{
    int index = 0;
    char * pline = read_line_msg(index);


    delete pline;
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
            char * p = (char *)malloc(len);
            strncpy(p,tep,len);
            p[len] = '\0';
            index ++;
            return p;
        }
    }
    //把位置移到换行符的下一个字符
    index ++;
}

