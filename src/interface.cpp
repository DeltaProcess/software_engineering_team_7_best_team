#include "cpptk.h"
#include "interface.h"

#include <iostream>

using namespace Tk;

Interface::Interface(char* arg) {
	init(arg);

	button(".b") -text("Say Hello") -command(this->hello);
	pack(".b") -padx(20) -pady(6);
			  
	runEventLoop();
}

void Interface::hello() {
	std::cout << "Hello, world!" << std::endl;
}
