#include "protocol.h"
#include "string.h"

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
}
