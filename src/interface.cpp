#include "cpptk.h"

Interface::Interface(char* arg) {
	this->init(arg)
}

void Interface::hello() {
	Tk::puts("Hello C++/Tk!");
}

void Interface::init(char* arg)
{
	Tk::init(arg);

	Tk::button(".b") -text("Say Hello") -command(hello);
	Tk::pack(".b") -padx(20) -pady(6);
			  
	Tk::runEventLoop();
}
