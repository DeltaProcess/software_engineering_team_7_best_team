#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include "audio.h"
#include "database.h"

class Interface {
	public:
		Interface(char* arg);
		void splash();
		static void importImage(std::string path, int size_x, int size_y);
		static void countdown(const std::string & music_path, int music_offset);
		static void setID();
		static void setName();
		static void setEqID();
		static void addRedPlayer();
		static void addGreenPlayer();
		static void addPlayer(Player::Team team);
		static void clearRedPlayers();
		static void clearGreenPlayers();
		static void clearAllPlayers();
		static void clearPlayers(bool red, bool green);
		static void refreshDisplay();
		static void start();
		static void startCheck();
		static void showPlayActionScreen();
		static void refreshScoreBoard();
		static void addGameEvent(int shooter, int target);
		static void refreshGameEvents();
		static void timer();
		static bool updateNetAddr();
		static std::vector<std::pair<std::string, int>> sortPlayers(std::map<int, Player> playerTeam);
	private:
		static std::string tmp_img;
		static std::string netaddr;
		static Audio audio;
};

#endif
