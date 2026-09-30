#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include "database.h"

class Interface {
	public:
		Interface(char* arg);
		void splash();
		void importImage(std::string path, int size_x, int size_y);
		
		static void enterID();
		static void enterName();
		static void enterEquipmentID();
		
		static void addRedPlayer();
		static void addGreenPlayer();
		static void addPlayer(Player::Team team);
		static void refreshDisplay();
		
	private:
		std::string tmp_img;
};

#endif
