#include "cpptk.h"
#include "interface.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>

using namespace Tk;
using namespace cv;

Interface::Interface(char* arg) {
	init(arg);
	
	this->tmp_img = "assets/images/tmp.png";
	
	// splash screen
	importImage("assets/images/logo.jpg", 300, 300);
	button(".b") -image(images(create, photo, "logo") -file(this->tmp_img));
	pack(".b") -expand(true) -fill("both");
	update();
	splash();
	
	// red and green team labels
	label(".l1") -width(40) -text("Red Team") -fg("red") -bg("black");
	label(".l2") -width(40) -text("Green Team") -fg("green") -bg("black");
	grid(configure, ".l1") -column(0) -row(0);
	grid(configure, ".l2") -column(1) -row(0);
	
	// red and green team text display
	textw(".t1") -width(40) -height(30) -fg("red") -bg("black");
	textw(".t2") -width(40) -height(30) -fg("green") -bg("black");
	grid(configure, ".t1") -column(0) -row(1);
	grid(configure, ".t2") -column(1) -row(1);
	
	// red and green team entries
	entry(".e1") -textvariable(this->str) -width(20) -invalidcommand("bell");
	entry(".e2") -textvariable(this->str) -width(20) -invalidcommand("bell");
	entry(".e3") -textvariable(this->str) -width(20) -invalidcommand("bell");
	entry(".e4") -textvariable(this->str) -width(20) -invalidcommand("bell");
	grid(configure, ".e1") -column(0) -row(2);
	grid(configure, ".e2") -column(0) -row(3);
	grid(configure, ".e3") -column(1) -row(2);
	grid(configure, ".e4") -column(1) -row(3);
	".e1" << insert(0, "id:");
	".e2" << insert(0, "codename:");
	".e3" << insert(0, "id:");
	".e4" << insert(0, "codename:");
	
	// add user
	button(".b") -text("Enter Player") -command(this->getPlayer);
	grid(configure, ".b") -column(0) -row(4);
	
	runEventLoop();
}

Interface::~Interface() {
	
}

// remove splash screen
void Interface::splash() {
	std::this_thread::sleep_for(std::chrono::seconds(3));
	destroy(".b");
	images(deleteimg, "logo");
}

// autoimporter for .jpg images
//
// use: you have to importImage(path, width, height)
// BEFORE the -file() option
void Interface::importImage(std::string path, int size_x, int size_y) {
	Mat src = imread(path);
	Mat dst;
	Size s(size_x, size_y);
	resize(src, dst, s);
	
	std::vector<uchar> buf;
	imencode(".png", dst, buf);
	
	std::ofstream ofs(this->tmp_img, std::ofstream::out);
	ofs.write(reinterpret_cast<const char*>(buf.data()), buf.size());
	ofs.close();
}

// get player information
std::string* Interface::getPlayer() {
	std::string* player = new std::string[2];
	return player;
}
