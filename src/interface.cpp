#include "interface.h"
#include "database.h"
#include "cpptk.h"

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <map>

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
	textw(".t1") -width(40) -height(30) -fg("white") -bg("#4d0000") -tabs("60 240");
	textw(".t2") -width(40) -height(30) -fg("white") -bg("#004d00") -tabs("60 240");
	grid(configure, ".t1") -column(0) -row(1);
	grid(configure, ".t2") -column(1) -row(1);
	".t1" << insert("0.0", "ID\tCodename\tEquipment\n");
	".t2" << insert("0.0", "ID\tCodename\tEquipment\n");
	".t1" << configure() -state("disabled");
	".t2" << configure() -state("disabled");



	// everything below the two team boxes
	frame(".f1");
	grid(configure, ".f1") -column(0) -row(3) -pady(10) -columnspan(2);
	
	// entry text box labels
	label(".f1.l1") -text("ID");
	label(".f1.l2") -text("Codename");
	label(".f1.l3") -text("Equipment ID");
	grid(configure, ".f1.l1") -column(0) -row(3);
	grid(configure, ".f1.l2") -column(1) -row(3);
	grid(configure, ".f1.l3") -column(2) -row(3);
	
	// team entries
	entry(".f1.e1") -width(20) -invalidcommand("bell");
	entry(".f1.e2") -width(20) -invalidcommand("bell");
	entry(".f1.e3") -width(20) -invalidcommand("bell");
	grid(configure, ".f1.e1") -column(0) -row(4) -padx(10);
	grid(configure, ".f1.e2") -column(1) -row(4) -padx(10);
	grid(configure, ".f1.e3") -column(2) -row(4) -padx(10);
	
	// lets the user hti enter to save changes/query database
	bind(".f1.e1", "<Return>", enterID);
	bind(".f1.e2", "<Return>", enterName);

	// add user
	button(".b1") -text("Add to Red") -command(this->addRedPlayer) -fg("white") -bg("#bf0000") -activebackground("#9c0000") -activeforeground("white");
	grid(configure, ".b1") -column(0) -row(4) -pady(10);

	button(".b2") -text("Add to Green") -command(this->addGreenPlayer) -fg("white") -bg("#00bf00") -activebackground("#009c00") -activeforeground("white");
	grid(configure, ".b2") -column(1) -row(4) -pady(10);

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

void Interface::enterID() {
	int id;
	try {
		id = std::stoi(".f1.e1" << get());
	} catch(const std::exception &e) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Enter a valid integer.") -messagetype(ok);	
		return;
	}
		if (Database::searchID(id) != "") {
			".f1.e2" << deletetext("0", end);
			".f1.e2" << insert("0", Database::searchID(id));
			focus(".f1.e3"); // focuses on equipment text box if codename is found
		} else {
			focus(".f1.e2");
		}
}

void Interface::enterName() {
	int id;
	try {
		id = std::stoi(".f1.e1" << get());
	} catch(const std::exception &e) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Enter a valid integer.") -messagetype(ok);
		return;
	}
	std::string codename = ".f1.e2" << get();
	if (!codename.empty()) {
		if (Database::searchID(id) == "") {
			Database::addPlayerToDatabase(id, codename);
		} else {
			Database::editCodename(id, codename);
		}
	}
	focus(".f1.e3");
}

// since the buttons cannot carry parameters i had to create two helper functions
void Interface::addRedPlayer() {
	addPlayer(Player::Team::RED);
}	
void Interface::addGreenPlayer() {
		addPlayer(Player::Team::GREEN);
}	

void Interface::addPlayer(Player::Team team) {
	int id;
	int eqID;
	try { // checks for valid id
		id = std::stoi(".f1.e1" << get());
	} catch(const std::exception &e) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Enter a valid integer for the ID.") -messagetype(ok);	
		focus(".f1.e1");
		return;
	}
	
	try { //checks for valid equipment id, also includes even and odd checks
		eqID = std::stoi(".f1.e3" << get());
		std::cout << std::to_string((eqID&team)) <<std::endl;
		if ((eqID&1) != team) { 
			if (team == Player::Team::RED) {
				tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Even Equipment Numbers belong to the Green Team.") -messagetype(ok);
			} else { 
				tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Odd Equipment Numbers belong to the Red Team.") -messagetype(ok);
			}
			return;
		}
	} catch(const std::exception &e) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Enter a valid integer for the Equipment ID.") -messagetype(ok);	
		focus(".f1.e3");
		return;
	}
	
	std::string codename = ".f1.e2" << get();
	if (codename.empty()) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Enter a Codename.") -messagetype(ok);	
		focus(".f1.e2");
		return;
	}
	
	// status will show a dialog box with the corresponding error and will focus on what box has it
	int status = Database::addPlayerToPlayers(id, codename, team, eqID);
	if (status == 1) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Current ID and Codename does not match database. Please hit enter in the codename box.") -messagetype(ok);
		focus(".f1.e2");
		return;
	} else if(status == 3) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Equipment ID already in use.") -messagetype(ok);	
		focus(".f1.e3");
		return;
	}
	
	refreshDisplay();
	
	".f1.e1" << deletetext("0", end);
	".f1.e2" << deletetext("0", end);
	".f1.e3" << deletetext("0", end);
	focus(".f1.e1");
	
}	


// this refreshes both teams display every time to account for people switching teams and equipment
void Interface::refreshDisplay() {
	std::map<int,Player> rosterRed = Database::getPlayers(Player::Team::RED);
	std::map<int,Player> rosterGreen = Database::getPlayers(Player::Team::GREEN);
	
	".t1" << configure() -state("normal");
	".t1" << deletetext("1.0", end);
	".t1" << insert("0.0", "ID\tCodename\tEquipment\n");
	for (auto player: rosterRed) {
		int eqID = player.first;
		Player p = player.second;
		 ".t1"  << insert("end", std::to_string(p.id) + "\t" + p.codename + "\t"+ std::to_string(eqID) + "\n");
	}
	".t1" << configure() -state("disabled");
	
	".t2" << configure() -state("normal");
	".t2" << deletetext("1.0", end);
	".t2" << insert("0.0", "ID\tCodename\tEquipment\n");
	for (auto player: rosterGreen) {
		int eqID = player.first;
		Player p = player.second;
		 ".t2" << insert("end", std::to_string(p.id) + "\t" + p.codename + "\t"+ std::to_string(eqID) + "\n");
	}
	".t2" << configure() -state("disabled");
}	

