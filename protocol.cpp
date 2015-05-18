#include "protocol.h"
#include "string.h"
#include "player.h"

#include <sys/types.h>  //socket()
#include <sys/socket.h> //socket(),bind()
#include <arpa/inet.h>  //Inet_pton(),htons()

#include <stdio.h>      //sprintf()
#include <stdlib.h>     //strcpy
#include <strings.h>    //bzero()
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
    //打开本地套接字
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0))== -1){
       cout << "socket() error!" << endl;
       return(-1);
    }

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

    if((connect(sock_fd, (struct sockaddr *)&servaddr, sizeof(servaddr))) == -1){
        cout << "connect() error!" << endl;
        return(-1);
    }
    return sock_fd;
}

int Protocol::startBroadCast()
{
    const int on = 1;
    //打开广播套接字
    if ((broadcast_fd = socket(AF_INET, SOCK_DGRAM, 0))== -1){
       cout << "socket() error!" << endl;
       return(-1);
    }

    //绑定本地IP和端口
    struct sockaddr_in cliaddr;
    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;  //设定客户端类型
    cliaddr.sin_port = htons(6011);//设定客户端端口
    if((bind(broadcast_fd, (struct sockaddr *)&cliaddr, sizeof(cliaddr))) == -1){
        cout << "UDP-bind() error!" << endl;
        return(-1);
    }

    //设定为接受广播消息
    setsockopt(broadcast_fd, SOL_SOCKET, SO_BROADCAST, &on, sizeof(on));

    return broadcast_fd;
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
        if ((len = recv(broadcast_fd, bufRecv, 500, 0)) != 0) {
            bufRecv[len]='\0';
            cout << bufRecv << strlen(bufRecv)<<endl;

            //进入消息处理阶段
            //game_over消息
            if(strcmp("game over",bufRecv) == 0){
                stop_game_over_msg(0);
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

bool Protocol::stop_game_over_msg(void*)
{
    gameStop = true;
    return true;
}
