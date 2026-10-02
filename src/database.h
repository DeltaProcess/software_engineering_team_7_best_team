#ifndef DATABASE_H
#define DATABASE_H

#include "player.h"

#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include <map>

class Database {
	public:
		static pqxx::connection& conn();
		static void printTable();
		static std::string searchID(int id);
		static void addPlayerToDatabase(int id, std::string codename);
		static void editCodename(int id, std::string codename);
		static int addPlayerToPlayers(int id, std::string codename, Player::Team team, int equipmentID);
		static std::map<int, Player> getPlayers(Player::Team team);
		static bool friendlyFire(int eqID1, int eqID2);
	private:
		static pqxx::connection c;
		static std::map<int, Player> players;
};

#endif
