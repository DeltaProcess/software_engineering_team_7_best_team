#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include "database.h"

class Interface {
	public:
		Interface(char* arg);
		~Interface();
		void splash();
		void importImage(std::string path, int size_x, int size_y);
		static void getPlayerRed();
		static void getPlayerGreen();
	private:
		std::string tmp_img;
		static std::string redTeamID;
		static std::string redTeamName;
		static std::string greenTeamID;
		static std::string greenTeamName;
		static Database database;
};

#endif
