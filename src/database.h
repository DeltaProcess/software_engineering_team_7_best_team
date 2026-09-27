#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <pqxx/pqxx>
#include <string>

class Database {
	public:
		Database();
		void printTable();
		std::string searchID(int id);
		void addPlayer(int id, std::string codename);

	private:
		pqxx::connection c;
};

#endif
