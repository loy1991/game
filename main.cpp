#include "protocol.h"
#include "player.h"

#include <stdlib.h>

#include <iostream>
using namespace std;


int main(int argc, char* argv[])
{
    if(argc != 6){
        cout << "Wrong useage!" << endl;
        cout << "please input arg like:" << endl;
        cout << "./game  192.168.0.1  1024  192.168.0.2  2048  6001" << endl;
        return(-1);
    }

    Player p(argv[1], atoi(argv[2]), argv[3],atoi(argv[4]), atoi(argv[5]));
    p.start_PlayCard();
    cout << "Hello World!" << endl;
    return 0;

}

