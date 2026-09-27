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
	
	this->tmp_img = "assets/images/tmp.gif";
	
	// splash screen
	importImage("assets/images/logo.jpg", 300, 300);
	button(".b") -image(images(create, photo, "logo") -file(this->tmp_img));
	pack(".b") -expand(true) -fill("both");
	splash();
	
	// red and green team labels
	label(".l1") -width(40) -text("Red Team") -fg("red") -bg("black");
	label(".l2") -width(40) -text("Green Team") -fg("red") -bg("black");
	grid(configure, ".l1") -column(0) -row(0);
	grid(configure, ".l2") -column(1) -row(0);
	
	// red and green team text display
	textw(".t1") -width(40) -height(40) -fg("red") -bg("black");
	textw(".t2") -width(40) -height(40) -fg("red") -bg("black");
	grid(configure, ".t1") -column(0) -row(1);
	grid(configure, ".t2") -column(1) -row(1);
	
	// red and green team entries
	entry(".e1") -textvariable(this->str) -width(40);
	entry(".e2") -textvariable(this->str) -width(40);
	grid(configure, ".e1") -column(0) -row(2);
	grid(configure, ".e2") -column(1) -row(2);
	
	runEventLoop();
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
