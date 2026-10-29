#ifndef PLAYER_H
#define PLAYER_H

#include <string>

struct Player {
	enum Team {
		GREEN = 0,
		RED = 1
	};
	int id;
	std::string codename;
	Team team;
	bool hasHitBase = false;
	int score = 0;
	int equipmentID;
};

#endif
