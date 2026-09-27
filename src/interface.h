#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>

class Interface {
	public:
		Interface(char* arg);
		~Interface();
		void splash();
		void importImage(std::string path, int size_x, int size_y);
		static std::string* getPlayer();
	private:
		std::string str;
		std::string tmp_img;
};

#endif
