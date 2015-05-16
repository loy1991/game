#include "protocol.h"

#include <iostream>
using namespace std;


int main(int argc, char* argv[])
{
    Protocol client("192.168.1.2", 1234, "192.168.1.110",23424, 89);
    client.startClient();
    cout << "Hello World!" << endl;
    return 0;
}

