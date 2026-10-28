#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include "database.h"

class Interface {
	public:
		Interface(char* arg);
		void splash();
		static void importImage(std::string path, int size_x, int size_y);
		static void countdown();
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
	private:
		static std::string tmp_img;
};

#endif
