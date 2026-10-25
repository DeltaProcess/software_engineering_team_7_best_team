#include "interface.h"
#include "udp/udp_broadcast.h"
#include "udp/udp_receive.h"
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

std::string Interface::tmp_img = "assets/images/tmp.png";

Interface::Interface(char* arg) {
	init(arg);
	
	// formatted window title
	wm("title", ".", "                Photon7");

	// splash screen
	importImage("assets/images/logo.jpg", 300, 300);
	button(".b") -image(images(create, photo, "logo") -file(tmp_img));
	pack(".b") -expand(true) -fill("both");
	update();
	splash();

	// red and green team labels
	label(".l1") -width(40) -text("Red Team") -fg("red") -bg("black");
	label(".l2") -width(40) -text("Green Team") -fg("green") -bg("black");
	grid(configure, ".l1") -column(0) -row(0);
	grid(configure, ".l2") -column(1) -row(0);

	// red and green team text display
	textw(".t1") -width(40) -height(20) -fg("white") -bg("#4d0000") -tabs("60 240") -highlightbackground("#0000fe") -highlightthickness(2);
	textw(".t2") -width(40) -height(20) -fg("white") -bg("#004d00") -tabs("60 240") -highlightbackground("#0000fe") -highlightthickness(2);
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
	entry(".f1.e1") -width(20) -invalidcommand("bell") -highlightbackground("#0000fe") -highlightthickness(2);
	entry(".f1.e2") -width(20) -invalidcommand("bell") -highlightbackground("#0000fe") -highlightthickness(2);
	entry(".f1.e3") -width(20) -invalidcommand("bell") -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.e1") -column(0) -row(4) -padx(10);
	grid(configure, ".f1.e2") -column(1) -row(4) -padx(10);
	grid(configure, ".f1.e3") -column(2) -row(4) -padx(10);

	// let user save changes/query database with 'enter' key
	bind(".f1.e1", "<Return>", setID);
	bind(".f1.e2", "<Return>", setName);

	// add user
	button(".f1.b1") -text("Add to Red") -command(this->addRedPlayer) -fg("white") -bg("#bf0000") -activebackground("#9c0000") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b1") -column(0) -row(5) -pady(10);

	button(".f1.b2") -text("Add to Green") -command(this->addGreenPlayer) -fg("white") -bg("#00bf00") -activebackground("#009c00") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b2") -column(2) -row(5) -pady(10);

	// start game
	button(".f1.b3") -text("Start") -command(this->start) -fg("white") -bg("black") -activebackground("gray") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b3") -column(1) -row(5) -pady(10);
	bind(".", "<F5>", start);
	
	// clear player buttons
	button(".f1.b4") -text("Clear Red") -command(this->clearRedPlayers) -fg("white") -bg("#6b3333") -activebackground("#4b2222") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b4") -column(0) -row(6) -pady(10);
	
	button(".f1.b5") -text("Clear Green") -command(this->clearGreenPlayers) -fg("white") -bg("#336b33") -activebackground("#224b22") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b5") -column(2) -row(6) -pady(10);
	
	button(".f1.b6") -text("Clear All") -command(this->clearAllPlayers) -fg("white") -bg("#414141") -activebackground("#222222") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b6") -column(1) -row(6) -pady(10);
	
	bind(".", "<F12>", clearAllPlayers);
	
	// recenter window
	update();
	int screen_width = std::stoi(winfo("screenwidth", "."));
	int screen_height = std::stoi(winfo("screenheight", "."));
	int window_width = std::stoi(winfo("reqwidth", "."));
	int window_height = std::stoi(winfo("reqheight", "."));
	int xpos = (screen_width - window_width) / 2;
	int ypos = (screen_height - window_height) / 2;
	wm(geometry, ".", std::to_string(window_width) + "x" + std::to_string(window_height) + "+" + std::to_string(xpos) + "+" + std::to_string(ypos));

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

	std::ofstream ofs(tmp_img, std::ofstream::out);
	ofs.write(reinterpret_cast<const char*>(buf.data()), buf.size());
	ofs.close();
}

void Interface::countdown() {
	// destroy grid elements
	destroy(".l1");
	destroy(".l2");
	destroy(".t1");
	destroy(".t2");
	destroy(".f1");
	
	// create and destroy countdown elements
	wm(geometry, ".", "=300x300");
	for(int i = 31; i > -1; i--) {
		std::string path = "assets/images/";
		if(i == 31) {
			path += "alert.jpg";
		} else {
			path += "countdown";
			path += std::to_string(i);
			path += ".jpg";
		}
		
		importImage(path, 300, 300);
		button(".b") -image(images(create, photo, "countdown") -file(tmp_img));
		pack(".b") -expand(true) -fill("both");
		update();
		std::this_thread::sleep_for(std::chrono::seconds(1));
		
		destroy(".b");
		images(deleteimg, "countdown");
	}
}

void Interface::start() {
	// displays countdown images
	// destroys everything but root to allow for action screen
	countdown();

	// display player action screen after countdown finishes
    showPlayActionScreen();
	
	// broadcast start
	std::thread rx(receive);
	rx.detach();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	std::string networkAddress = "127.0.0.1";
	broadcast(networkAddress.c_str());
}

void Interface::showPlayActionScreen() {
    // Get players already entered for each team
    std::map<int, Player> rosterRed =
        Database::getPlayers(Player::Team::RED);

    std::map<int, Player> rosterGreen =
        Database::getPlayers(Player::Team::GREEN);

    // Title
    // label(".paTitle")
    //     -text("PLAYER ACTION")
    //     -fg("white")
    //     -bg("black"); 
										// Didn't like how this label looked probably will delete later
    // grid(configure, ".paTitle")
    //     -column(0)
    //     -row(0)
    //     -columnspan(2)
    //     -pady(10);

    // Team titles
    label(".paRedLabel")
        -width(40)
        -text("RED TEAM")
        -fg("red")
        -bg("black");

    label(".paGreenLabel")
        -width(40)
        -text("GREEN TEAM")
        -fg("green")
        -bg("black");

    grid(configure, ".paRedLabel")
        -column(0)
        -row(1);

    grid(configure, ".paGreenLabel")
        -column(1)
        -row(1);

    // Player displays
    textw(".paRed")
        -width(40)
        -height(10)
        -fg("white")
        -bg("#4d0000")
        -tabs("200")
        -highlightbackground("#0000fe")
        -highlightthickness(2);

    textw(".paGreen")
        -width(40)
        -height(10)
        -fg("white")
        -bg("#004d00")
        -tabs("200")
        -highlightbackground("#0000fe")
        -highlightthickness(2);

    grid(configure, ".paRed")
        -column(0)
        -row(2);

    grid(configure, ".paGreen")
        -column(1)
        -row(2);

    // Headers
    ".paRed" << insert("0.0", "Codename\tScore\n");
    ".paGreen" << insert("0.0", "Codename\tScore\n");

    // Red players
    for (auto player : rosterRed) {
        Player p = player.second;

        ".paRed" << insert(
            "end",
            p.codename + "\t0\n"
        );
    }

    // Green players
    for (auto player : rosterGreen) {
        Player p = player.second;

        ".paGreen" << insert(
            "end",
            p.codename + "\t0\n"
        );
    }

    ".paRed" << configure() -state("disabled");
    ".paGreen" << configure() -state("disabled");

    // Team scores
    label(".paRedScore")
        -text("Red Team Score: 0")
        -fg("red")
        -bg("black");

    label(".paGreenScore")
        -text("Green Team Score: 0")
        -fg("green")
        -bg("black");

    grid(configure, ".paRedScore")
        -column(0)
        -row(3)
        -pady(10);

    grid(configure, ".paGreenScore")
        -column(1)
        -row(3)
        -pady(10);

    // Play-by-play area
    label(".paActionLabel")
        -text("Current Game Action")
        -fg("white")
        -bg("black");

    grid(configure, ".paActionLabel")
        -column(0)
        -row(4)
        -columnspan(2);

    textw(".paAction")
        -width(80)
        -height(10)
        -fg("white")
        -bg("black")
        -highlightbackground("#0000fe")
        -highlightthickness(2);

    grid(configure, ".paAction")
        -column(0)
        -row(5)
        -columnspan(2);

    ".paAction" << configure() -state("disabled");

    // Timer placeholder
    label(".paTimer")
        -text("Time Remaining: --:--")
        -fg("white")
        -bg("black");

    grid(configure, ".paTimer")
        -column(0)
        -row(6)
        -columnspan(2)
        -pady(10);

    // Resize and center window
    update();

    int screen_width = std::stoi(winfo("screenwidth", "."));
    int screen_height = std::stoi(winfo("screenheight", "."));
    int window_width = std::stoi(winfo("reqwidth", "."));
    int window_height = std::stoi(winfo("reqheight", "."));

    int xpos = (screen_width - window_width) / 2;
    int ypos = (screen_height - window_height) / 2;

    wm(
        geometry,
        ".",
        std::to_string(window_width) + "x" +
        std::to_string(window_height) + "+" +
        std::to_string(xpos) + "+" +
        std::to_string(ypos)
    );
}

void Interface::setID() {
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
		// focus on equipment text box if codename is found
		focus(".f1.e3");
	} else {
		focus(".f1.e2");
	}
}

void Interface::setName() {
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

// red helper function
void Interface::addRedPlayer() {
	addPlayer(Player::Team::RED);
}

// green helper function
void Interface::addGreenPlayer() {
	addPlayer(Player::Team::GREEN);
}

void Interface::addPlayer(Player::Team team) {
	int id;
	int eqID;

	// check for valid id
	try {
		id = std::stoi(".f1.e1" << get());
	} catch(const std::exception &e) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Enter a valid integer for the ID.") -messagetype(ok);	
		focus(".f1.e1");
		return;
	}

	// check for valid equipment id, also includes even and odd checks
	try {
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

void Interface::clearRedPlayers() {
	Interface::clearPlayers(true, false);
}

void Interface::clearGreenPlayers() {
	Interface::clearPlayers(false, true);
}

void Interface::clearAllPlayers() {
	Interface::clearPlayers(true, true);
}

void Interface::clearPlayers(bool red, bool green) {
	Database::clearPlayers(red, green);
	
	refreshDisplay();
	
	".f1.e1" << deletetext("0", end);
	".f1.e2" << deletetext("0", end);
	".f1.e3" << deletetext("0", end);
	focus(".f1.e1");
}


// refresh both teams displays
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

