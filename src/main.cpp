#include "interface.h"
#include "udp/udp_broadcast.h"
#include "udp/udp_receive.h"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

int main(int argc, char** argv)
{
     std::string networkAddress;

     std::cout << "Type the network address you'd like to use: ";
     std::cin >> networkAddress;
     std::cout << "Using network: " << networkAddress << std::endl;
	 
	 std::thread rx(receive);
	 rx.detach();
	 std::this_thread::sleep_for(std::chrono::milliseconds(200));
	 
     broadcast(networkAddress.c_str());

     Interface* interface = new Interface(argv[0]);

     return 0;
}
