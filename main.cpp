#include "protocol.h"

#include <iostream>
using namespace std;


int main(int argc, char* argv[])
{
    Protocol client("127.0.0.1", 6000, "127.0.0.1",6001, 1111);
    client.startClient();
    client.ptos_reg_msg("good");
    cout << "Hello World!" << endl;
    return 0;
}

