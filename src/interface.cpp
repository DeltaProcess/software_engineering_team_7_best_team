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


//static variables and objects because tk stuff
std::string Interface::redTeamID;
std::string Interface::redTeamName;
std::string Interface::greenTeamID;
std::string Interface::greenTeamName;
Database Interface::database;



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
	".t1" << configure() -state("disabled");
	".t2" << configure() -state("disabled");

	
	
	
	
	// red and green team entries
<<<<<<< HEAD
	entry(".e1") -textvariable(this->redTeamID) -width(20) -invalidcommand("bell");
	entry(".e2") -textvariable(this->redTeamName) -width(20) -invalidcommand("bell");
	entry(".e3") -textvariable(this->greenTeamID) -width(20) -invalidcommand("bell");
	entry(".e4") -textvariable(this->greenTeamName) -width(20) -invalidcommand("bell");
=======
	entry(".e1") -textvariable(this->str) -width(20) -invalidcommand("bell");
	entry(".e2") -textvariable(this->str) -width(20) -invalidcommand("bell");
	entry(".e3") -textvariable(this->str) -width(20) -invalidcommand("bell");
	entry(".e4") -textvariable(this->str) -width(20) -invalidcommand("bell");
>>>>>>> c61ac978f4f7577dbb3df5d6754806a031c378a8
	grid(configure, ".e1") -column(0) -row(2);
	grid(configure, ".e2") -column(0) -row(3);
	grid(configure, ".e3") -column(1) -row(2);
	grid(configure, ".e4") -column(1) -row(3);
	".e1" << insert(0, "id:");
	".e2" << insert(0, "codename:");
	".e3" << insert(0, "id:");
	".e4" << insert(0, "codename:");
	
	// add user
<<<<<<< HEAD
	button(".b1") -text("Enter Player") -command(this->getPlayerRed);
	grid(configure, ".b1") -column(0) -row(4);
	
	button(".b2") -text("Enter Player") -command(this->getPlayerGreen);
	grid(configure, ".b2") -column(1) -row(4);
=======
	button(".b") -text("Enter Player") -command(this->getPlayer);
	grid(configure, ".b") -column(0) -row(4);
>>>>>>> c61ac978f4f7577dbb3df5d6754806a031c378a8
	
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

<<<<<<< HEAD
//red team button
void Interface::getPlayerRed() {
	int id = std::stoi(redTeamID);
	if (id % 2 != 1){ //only odd ids(?)
		return;
	}
	if (database.searchID(id) != ""){ //if the database already has a match
		".t1" << configure() -state("normal");
		".t1" << insert("0.0", redTeamID + "\t\t" + database.searchID(id) + "\n");
	}
	else if(redTeamName != ""){ //if not, and the codename box isn't empty
		database.addPlayer(id, redTeamName);
		".t1" << configure() -state("normal");
		".t1" << insert("0.0", redTeamID + "\t\t" + database.searchID(id) + "\n");
	}

	".t1" << configure() -state("disabled");	
}

//green team button, same as red but green
void Interface::getPlayerGreen() {
	int id = std::stoi(greenTeamID);
	if (id % 2 != 0){
		return;
	}
	if (database.searchID(id) != ""){
		".t2" << configure() -state("normal");
		".t2" << insert("0.0", greenTeamID + "\t\t" + database.searchID(id) + "\n");
	}
	else if(greenTeamName != ""){
		database.addPlayer(id, greenTeamName);
		".t1" << configure() -state("normal");
		".t2" << insert("0.0", greenTeamID + "\t\t" + greenTeamName + "\n");
	}

	".t1" << configure() -state("disabled");	
=======
// get player information
std::string* Interface::getPlayer() {
	std::string* player = new std::string[2];
	return player;
>>>>>>> c61ac978f4f7577dbb3df5d6754806a031c378a8
}
