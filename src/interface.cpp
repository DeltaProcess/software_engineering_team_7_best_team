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
#include <random>
#include <mutex>

using namespace Tk;
using namespace cv;

std::string Interface::tmp_img = "assets/images/tmp.png";
std::string Interface::netaddr = "127.0.0.1";
Audio Interface::audio;
bool Interface::firstRun = true;

// Events received by the UDP thread wait here until the GUI refreshes them.
namespace {
    std::mutex gameEventMutex;
    std::vector<std::pair<int, int>> pendingGameEvents;
}

// struct to hold onto each track's data
struct Track
{
	std::string path;
	int start_time;
};

Interface::Interface(char* arg) {
	init(arg);
	audio.init();
	splash();
	showPlayerEntry();
}

// Display splash screen
void Interface::splash() {
	importImage("assets/images/logo.jpg", 300, 300);
	button(".b") -image(images(create, photo, "logo") -file(tmp_img));
	pack(".b") -expand(true) -fill("both");
	update();
	std::this_thread::sleep_for(std::chrono::seconds(3));

	destroy(".b");
	images(deleteimg, "logo");
}

void Interface::showPlayerEntry() {
	// Formatted window title
	wm("title", ".", "                Photon7");

	// Red and green team labels
	label(".l1") -width(80) -text("Red Team") -fg("red") -bg("black");
	label(".l2") -width(80) -text("Green Team") -fg("green") -bg("black");
	grid(configure, ".l1") -column(0) -row(0);
	grid(configure, ".l2") -column(1) -row(0);

	// Red and green team text display
	textw(".t1") -width(80) -height(20) -fg("white") -bg("#4d0000") -tabs("60 240") -highlightbackground("#0000fe") -highlightthickness(2);
	textw(".t2") -width(80) -height(20) -fg("white") -bg("#004d00") -tabs("60 240") -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".t1") -column(0) -row(1);
	grid(configure, ".t2") -column(1) -row(1);
	".t1" << insert("0.0", "ID\t\t      Codename\t                  Equipment\n");
	".t2" << insert("0.0", "ID\t\t      Codename\t                  Equipment\n");
	".t1" << configure() -state("disabled");
	".t2" << configure() -state("disabled");

	// Everything below the two team boxes
	frame(".f1");
	grid(configure, ".f1") -column(0) -row(3) -pady(10) -columnspan(2);

	// Entry text box labels
	label(".f1.l1") -text("ID");
	label(".f1.l2") -text("Codename");
	label(".f1.l3") -text("Equipment ID");
	grid(configure, ".f1.l1") -column(0) -row(3);
	grid(configure, ".f1.l2") -column(1) -row(3) -padx(200);
	grid(configure, ".f1.l3") -column(2) -row(3);

	// Team entries
	entry(".f1.e1") -width(20) -invalidcommand("bell") -highlightbackground("#0000fe") -highlightthickness(2);
	entry(".f1.e2") -width(20) -invalidcommand("bell") -highlightbackground("#0000fe") -highlightthickness(2);
	entry(".f1.e3") -width(20) -invalidcommand("bell") -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.e1") -column(0) -row(4) -padx(10);
	grid(configure, ".f1.e2") -column(1) -row(4) -padx(10);
	grid(configure, ".f1.e3") -column(2) -row(4) -padx(10);

	// Let user save changes/query database with 'enter' key
	bind(".f1.e1", "<Return>", setID);
	bind(".f1.e2", "<Return>", setName);
	bind(".f1.e3", "<Return>", setEqID);

	// Add user
	button(".f1.b1") -text("Add to Red") -command(addRedPlayer) -fg("white") -bg("#bf0000") -activebackground("#9c0000") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b1") -column(0) -row(5) -pady(10);

	button(".f1.b2") -text("Add to Green") -command(addGreenPlayer) -fg("white") -bg("#00bf00") -activebackground("#009c00") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b2") -column(2) -row(5) -pady(10);

	// Start game
	button(".f1.b3") -text("Start") -command(startCheck) -fg("white") -bg("black") -activebackground("gray") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b3") -column(1) -row(5) -pady(10);
	bind(".", "<F5>", startCheck);
	
	// Clear player buttons
	button(".f1.b4") -text("Clear Red") -command(clearRedPlayers) -fg("white") -bg("#6b3333") -activebackground("#4b2222") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b4") -column(0) -row(6) -pady(10);
	
	button(".f1.b5") -text("Clear Green") -command(clearGreenPlayers) -fg("white") -bg("#336b33") -activebackground("#224b22") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b5") -column(2) -row(6) -pady(10);
	
	button(".f1.b6") -text("Clear All") -command(clearAllPlayers) -fg("white") -bg("#414141") -activebackground("#222222") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b6") -column(1) -row(6) -pady(10);
	
	bind(".", "<F12>", clearAllPlayers);
	
	// Change network address
	label(".f1.l4") -text("Network Address:");
	grid(configure, ".f1.l4") -column(0) -row(7) -pady(10);
	
	entry(".f1.e4") -width(20) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.e4") -column(1) -row(7) -pady(10);
	".f1.e4" << insert("0", netaddr);
	
	button(".f1.b7") -text("Change Address") -command(updateNetAddr) -fg("white") -bg("#aa65b5") -activebackground("#623869") -activeforeground("white") -width(10) -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".f1.b7") -column(2) -row(7) -pady(10);
	
	// Recenter window
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

// Autoimporter for .jpg images
//
// Use: run importImage(path, width, height) BEFORE -file() option
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

void Interface::countdown(const std::string & music_path, int music_offset) {
	// Destroy grid elements
	destroy(".l1");
	destroy(".l2");
	destroy(".t1");
	destroy(".t2");
	destroy(".f1");
	
	bool music_started = false;

	// Create and destroy countdown elements
	wm(geometry, ".", "=300x300");
	"." << configure() -background("black");
	for (int i = 31; i > -1; i--) {
		std::string path = "assets/images/";
		if (i == 31) {
			path += "alert.jpg";
		} else {
			path += "countdown";
			path += std::to_string(i);
			path += ".jpg";
		}
		
		// Play music
		if(!music_started && (31 - i) >= music_offset)
		{
			audio.play(music_path);
			music_started = true;
		}

		importImage(path, 300, 300);
		button(".b") -image(images(create, photo, "countdown") -file(tmp_img));
		pack(".b") -expand(true) -fill("both");
		update();
		std::this_thread::sleep_for(std::chrono::seconds(1));

		destroy(".b");
		images(deleteimg, "countdown");

		// Recenter window
		update();
		int screen_width = std::stoi(winfo("screenwidth", "."));
		int screen_height = std::stoi(winfo("screenheight", "."));
		int window_width = std::stoi(winfo("reqwidth", "."));
		int window_height = std::stoi(winfo("reqheight", "."));
		int xpos = (screen_width - window_width) / 2;
		int ypos = (screen_height - window_height) / 2;
		wm(geometry, ".", std::to_string(window_width) + "x" + std::to_string(window_height) + "+" + std::to_string(xpos) + "+" + std::to_string(ypos));
	}
	"." << configure() -background("gray");
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
		// Focus on equipment text box if codename is found
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

void Interface::setEqID(){
	int id;
	try {
		id = std::stoi(".f1.e3" << get());
		broadcast(netaddr.c_str(), std::to_string(id).c_str());
	} catch(const std::exception &e) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Enter a valid integer for the Equipment ID.") -messagetype(ok);	
		focus(".f1.e3");
		return;
	}
	if (id&1){
		addPlayer(Player::Team::RED);
	} else {
		addPlayer(Player::Team::GREEN);
	}
}

// Red helper function
void Interface::addRedPlayer() {
	addPlayer(Player::Team::RED);
}

// Green helper function
void Interface::addGreenPlayer() {
	addPlayer(Player::Team::GREEN);
}

void Interface::addPlayer(Player::Team team) {
	int id;
	int eqID;

	// Check for valid id
	try {
		id = std::stoi(".f1.e1" << get());
	} catch(const std::exception &e) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Enter a valid integer for the ID.") -messagetype(ok);	
		focus(".f1.e1");
		return;
	}

	// Check for valid equipment id, also includes even and odd checks
	try {
		eqID = std::stoi(".f1.e3" << get());
		broadcast(netaddr.c_str(), std::to_string(eqID).c_str());
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

	// Status will show a dialog box with the corresponding error and will focus on what box has it
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

void Interface::start() {
	// Track list
	static const std::vector<Track> tracklist = {
		{"assets/audio/tracks/Track01.mp3", 16},
		{"assets/audio/tracks/Track02.mp3", 15},
		{"assets/audio/tracks/Track03.mp3", 16},
		{"assets/audio/tracks/Track04.mp3", 15},
		{"assets/audio/tracks/Track05.mp3", 15},
		{"assets/audio/tracks/Track06.mp3", 16},
		{"assets/audio/tracks/Track07.mp3", 15},
		{"assets/audio/tracks/Track08.mp3", 15}
	};
	
	// Randomly select audio
	static std::mt19937 rng{std::random_device{}()};
	std::uniform_int_distribution<size_t> pick(0, tracklist.size() - 1);
	const Track & chosen_track = tracklist[pick(rng)];
	
	// Displays countdown images
	// Destroy everything but root for play action screen
	countdown(chosen_track.path, chosen_track.start_time);
	
	// Broadcast start
	if(firstRun) {
		std::thread rx(receive);
		rx.detach();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	std::string message = "202";
	broadcast(netaddr.c_str(), message);
	
	// Display player action screen after countdown finishes
    showPlayActionScreen();
}

void Interface::startCheck() {
	if (Database::getNumofPlayers() < 2) {
		tk_messageBox() -defaultbutton("ok") -icon("error") -messagetext("Need at least 2 players to start.") -messagetype(ok);
		return;
	}
	start();
}

void Interface::showPlayActionScreen() {
    // Get players already entered for each team
    std::map<int, Player> rosterRed = Database::getPlayers(Player::Team::RED);
    std::map<int, Player> rosterGreen = Database::getPlayers(Player::Team::GREEN);

	// Load the base icon once for use beside players who hit a base
	importImage("assets/images/baseicon.jpg", 24, 18);

	// Delete an old copy if another game was previously played
	eval("catch {image delete baseIcon}");

	images(create, photo, "baseIcon") -file(tmp_img);

	// Get each team's current score
	int scoreRed = Database::getTeamScore(Player::Team::RED);
	int scoreGreen = Database::getTeamScore(Player::Team::GREEN);

    // Titles
    label(".paRedLabel") -width(40) -text("RED TEAM") -fg("red") -bg("black");
	grid(configure, ".paRedLabel") -column(0) -row(1);

    label(".paGreenLabel") -width(40) -text("GREEN TEAM") -fg("green") -bg("black");
	grid(configure, ".paGreenLabel") -column(1) -row(1);

    // Player displays
    textw(".paRed") -width(80) -height(20) -fg("white") -bg("#4d0000") -tabs("200") -highlightbackground("#0000fe") -highlightthickness(2);
	grid(configure, ".paRed") -column(0) -row(2);

    textw(".paGreen") -width(80) -height(20) -fg("white") -bg("#004d00") -tabs("200") -highlightbackground("#0000fe") -highlightthickness(2);
    grid(configure, ".paGreen") -column(1) -row(2);

    // Headers
    ".paRed" << insert("0.0", "Codename\t\t                          Score\n");
    ".paGreen" << insert("0.0", "Codename\t\t                          Score\n");

    // Red players
    for (auto player : rosterRed) {
        Player p = player.second;
		// Put base icon to the left of the codename
		// if this player has hit a base.
		if (p.hasHitBase) {
			eval(".paRed image create end -image baseIcon");
			".paRed" << insert("end", " ");
		}
		".paRed" << insert("end", p.codename + "\t\t                          " + std::to_string(p.score) + "\n");
	}
    ".paRed" << configure() -state("disabled");

    // Green players
    for (auto player : rosterGreen) {
        Player p = player.second;
 
		if (p.hasHitBase) {
			eval(".paGreen image create end -image baseIcon");
			".paGreen" << insert("end", " ");
		}
        ".paGreen" << insert("end", p.codename + "\t\t                          " + std::to_string(p.score) + "\n");
	}
    ".paGreen" << configure() -state("disabled");

    // Team scores
    label(".paRedScore") -text("Red Team Score: " + std::to_string(scoreRed)) -fg("red") -bg("black");
	grid(configure, ".paRedScore") -column(0) -row(3) -pady(10);

    label(".paGreenScore") -text("Green Team Score: " + std::to_string(scoreGreen)) -fg("green") -bg("black");
    grid(configure, ".paGreenScore") -column(1) -row(3) -pady(10);

    // Play-by-play area
    label(".paActionLabel") -text("Current Game Action") -fg("white") -bg("black");
    grid(configure, ".paActionLabel") -column(0) -row(4) -columnspan(2);

    textw(".paAction") -width(80) -height(10) -fg("white") -bg("black") -highlightbackground("#0000fe") -highlightthickness(2);
    grid(configure, ".paAction") -column(0) -row(5) -columnspan(2);
    ".paAction" << configure() -state("disabled");

    // Timer placeholder
    label(".paTimer") -text("Time Remaining: --:--") -fg("white") -bg("black");
    grid(configure, ".paTimer") -column(0) -row(6) -columnspan(2) -pady(10);

    // Resize and center window
    update();

    int screen_width = std::stoi(winfo("screenwidth", "."));
    int screen_height = std::stoi(winfo("screenheight", "."));
    int window_width = std::stoi(winfo("reqwidth", "."));
    int window_height = std::stoi(winfo("reqheight", "."));

    int xpos = (screen_width - window_width) / 2;
    int ypos = (screen_height - window_height) / 2;

    wm(geometry, ".", std::to_string(window_width) + "x" + std::to_string(window_height) + "+" + std::to_string(xpos) + "+" + std::to_string(ypos));
    timer();
}

void Interface::returnPlayerEntry() {
	// Remove all but root to return to player entry
	destroy(".paRedLabel");
	destroy(".paGreenLabel");
	destroy(".paRed");
	destroy(".paGreen");
	destroy(".paRedScore");
	destroy(".paGreenScore");
	destroy(".paActionLabel");
	destroy(".paAction");
	destroy(".paTimer");
	destroy(".paReturn");
	// Remove players from database
	Database::clearPlayers(true, true);
	firstRun = false;
	showPlayerEntry();
}

void Interface::addGameEvent(int shooter, int target) {
    // This function is called by the UDP receive thread.
    // Store the event so the main GUI thread can display it safely.
    std::lock_guard<std::mutex> lock(gameEventMutex);
    pendingGameEvents.push_back({shooter, target});
}

void Interface::timer() {
	//361
	for (int i = 31; i >= 0; i--) {
		int min = i/60;
		int sec = i%60;
		std::string timer = "Time Remaining: " + std::string((min<10)?"0":"") + std::to_string(min) + ":" + std::string((sec<10)?"0":"") + std::to_string(sec);
		".paTimer" << configure() -text(timer);
		
		int scoreRed = Database::getTeamScore(Player::Team::RED);
		int scoreGreen = Database::getTeamScore(Player::Team::GREEN);
		if(scoreRed > scoreGreen) {
			".paGreenScore" << configure() -foreground("white");
			".paRedScore" << configure() -foreground((i%2==0)?"red":"white");
		} else {
			".paRedScore" << configure() -foreground("white");
			".paGreenScore" << configure() -foreground((i%2==0)?"green":"white");
		}
		
		refreshScoreBoard();
		refreshGameEvents();
		update();
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	// broadcasts 221 three times
	broadcast(netaddr.c_str(), "221");
	broadcast(netaddr.c_str(), "221");
	broadcast(netaddr.c_str(), "221");
	
	// Return button
    button(".paReturn") -text("Return to Player Entry") -command(returnPlayerEntry) -fg("white") -bg("#bf0000");
    grid(configure, ".paReturn") -column(0) -row(7);
    update();
}

void Interface::refreshScoreBoard() {
	// Get players already entered for each team
    std::map<int, Player> rosterRed = Database::getPlayers(Player::Team::RED);
    std::map<int, Player> rosterGreen = Database::getPlayers(Player::Team::GREEN);

	// get the sorted by scores lists
	std::vector<std::tuple<std::string, int, bool>> teamRed = sortPlayers(rosterRed); 
	std::vector<std::tuple<std::string, int, bool>> teamGreen = sortPlayers(rosterGreen); 

	// Get each team's current score
	int scoreRed = Database::getTeamScore(Player::Team::RED);
	int scoreGreen = Database::getTeamScore(Player::Team::GREEN);

	// Temporarily reenable labels
	".paRed" << configure() -state("normal");
	".paGreen" << configure() -state("normal");

	// Clear text in labels
	".paRed" << deletetext("1.0", "end");
	".paGreen" << deletetext("1.0", "end");

	// Headers
    ".paRed" << insert("0.0", "Codename\t\t                        Score\n");
    ".paGreen" << insert("0.0", "Codename\t\t                        Score\n");

	 // Red players
    for (auto p : teamRed) {
		if (std::get<2>(p)) {
			eval(".paRed image create end -image baseIcon");
			".paRed" << insert("end", " ");
		}	
        ".paRed" << insert("end", std::get<0>(p) + "\t\t                        " + std::to_string(std::get<1>(p)) + "\n");
   }
    ".paRed" << configure() -state("disabled");

    // Green players
    for (auto p : teamGreen) {
		if (std::get<2>(p)) {
			eval(".paGreen image create end -image baseIcon");
			".paGreen" << insert("end", " ");
		}
		 ".paGreen" << insert("end", std::get<0>(p) + "\t\t                        " + std::to_string(std::get<1>(p)) + "\n");
	}
	
    ".paGreen" << configure() -state("disabled");

	// Refresh scores
	".paRedScore" << configure() -text("Red Team Score: " + std::to_string(scoreRed));
	".paGreenScore" << configure() -text("Green Team Score: " + std::to_string(scoreGreen));
}

// Refresh team displays
void Interface::refreshDisplay() {
	std::map<int,Player> rosterRed = Database::getPlayers(Player::Team::RED);
	std::map<int,Player> rosterGreen = Database::getPlayers(Player::Team::GREEN);

	".t1" << configure() -state("normal");
	".t1" << deletetext("1.0", end);
	".t1" << insert("0.0", "ID\t\t      Codename\t                  Equipment\n");
	for (auto player: rosterRed) {
		int eqID = player.first;
		Player p = player.second;
		 ".t1"  << insert("end", std::to_string(p.id) + "\t\t      " + p.codename + "\t                  "+ std::to_string(eqID) + "\n");
	}
	".t1" << configure() -state("disabled");

	".t2" << configure() -state("normal");
	".t2" << deletetext("1.0", end);
	".t2" << insert("0.0", "ID\t\t      Codename\t                  Equipment\n");
	for (auto player: rosterGreen) {
		int eqID = player.first;
		Player p = player.second;
		 ".t2" << insert("end", std::to_string(p.id) + "\t\t      " + p.codename + "\t                  "+ std::to_string(eqID) + "\n");
	}
	".t2" << configure() -state("disabled");
}

void Interface::refreshGameEvents() {
    // Take all pending events out of the queue.
    std::vector<std::pair<int, int>> events;

    {
        std::lock_guard<std::mutex> lock(gameEventMutex);
        events.swap(pendingGameEvents);
    }

    // Nothing new happened since the last refresh.
    if (events.empty()) {
        return;
    }

    // Get current players so equipment IDs can be changed
    // into readable codenames.
    std::map<int, Player> rosterRed =
        Database::getPlayers(Player::Team::RED);

    std::map<int, Player> rosterGreen =
        Database::getPlayers(Player::Team::GREEN);

    // Helper for finding a player by equipment ID.
    auto findPlayer = [&](int equipmentID) -> const Player* {
        auto redPlayer = rosterRed.find(equipmentID);

        if (redPlayer != rosterRed.end()) {
            return &redPlayer->second;
        }

        auto greenPlayer = rosterGreen.find(equipmentID);

        if (greenPlayer != rosterGreen.end()) {
            return &greenPlayer->second;
        }

        return nullptr;
    };

    // Temporarily enable the text box so we can add text.
    ".paAction" << configure() -state("normal");

    for (const auto& event : events) {
        int shooterID = event.first;
        int targetID = event.second;

        const Player* shooter = findPlayer(shooterID);

        std::string shooterName;

        if (shooter != nullptr) {
            shooterName = shooter->codename;
        } else {
            shooterName = "Equipment " + std::to_string(shooterID);
        }

        std::string eventText;

        // Target 43 means the Green base was hit.
        if (targetID == 43) {
            eventText =
                shooterName + " hit the Green Base! (+100)";
        }

        // Target 53 means the Red base was hit.
        else if (targetID == 53) {
            eventText =
                shooterName + " hit the Red Base! (+100)";
        }

        // Normal player hit or friendly fire.
        else {
            const Player* target = findPlayer(targetID);

            std::string targetName;

            if (target != nullptr) {
                targetName = target->codename;
            } else {
                targetName = "Equipment " + std::to_string(targetID);
            }

            // Same team = friendly fire.
            if (shooter != nullptr &&
                target != nullptr &&
                shooter->team == target->team) {

                eventText =
                    "FRIENDLY FIRE: " +
                    shooterName +
                    " hit " +
                    targetName;
            }

            // Players are on opposing teams.
            else {
                eventText =
                    shooterName +
                    " hit " +
                    targetName;
            }
        }

        // Add this event to the bottom of the action window.
        ".paAction" << insert("end", eventText + "\n");
    }

    // Automatically keep the newest event visible.
    ".paAction" << see("end");

    // Prevent the user from manually editing the action log.
    ".paAction" << configure() -state("disabled");
}

bool Interface::updateNetAddr()
{
	std::string addr = ".f1.e4" << get();
	netaddr = addr;
	return true;
}

// sorts players by score to display from highest to lowest
std::vector<std::tuple<std::string, int, bool>> Interface::sortPlayers(std::map<int, Player> playerTeam){
	std::vector<std::tuple<std::string, int, bool>> sortingTeam;
	for (auto player : playerTeam) { // copies map to vector
        Player p = player.second;
        sortingTeam.push_back({p.codename, p.score, p.hasHitBase});
    }
    for (int m = 0; m < sortingTeam.size(); m++){
		bool swap = false;
		for (int h = 0; (h + 1)< sortingTeam.size()- m; h++){
			if (std::get<1>(sortingTeam[h]) < std::get<1>(sortingTeam[h+1])){
				std::swap(sortingTeam[h], sortingTeam[h+1]);
				swap = true;
			}
		}
		if (!swap){
			break;
		}
	}
	return sortingTeam;
}

