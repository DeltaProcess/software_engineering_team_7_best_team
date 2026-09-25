#include "interface.h"
#include "udp/udp_broadcast.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
     std::string networkAddress;

     std::cout << "Type the network address you'd like to use: ";
     std::cin >> networkAddress;
     std::cout << "Using network: " << networkAddress << std::endl;

     broadcast(networkAddress.c_str());

     
     Interface* interface = new Interface(argv[0]);

     return 0;
}
