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
<<<<<<< HEAD
		static void getPlayerRed();
		static void getPlayerGreen();
		
=======
		static std::string* getPlayer();
>>>>>>> c61ac978f4f7577dbb3df5d6754806a031c378a8
	private:
		std::string str;
		std::string tmp_img;
		static std::string redTeamID;
		static std::string redTeamName;
		static std::string greenTeamID;
		static std::string greenTeamName;
		static Database database;

		
};

#endif
