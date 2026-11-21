#include "database.h"

using namespace std;

std::map<int, Player> Database::players;

pqxx::connection& Database::conn() {
	static pqxx::connection c("host=/var/run/postgresql port=5432 dbname=photon target_session_attrs=read-write");
	return c;
}

void Database::printTable() {
	try {
		// starts a query
		pqxx::work tx(conn());

		// gets all columns
        pqxx::result res = tx.exec("SELECT * FROM players");

        // prints column names
        for (pqxx::row_size_type col = 0; col < res.columns(); col++) {
            cout << res.column_name(col) << "\t";
        }
        cout << "\n";

        // prints every row
        for (pqxx::result::size_type r = 0; r < res.size(); r++) {
			for (pqxx::row_size_type c = 0; c < res.columns(); c++) {
				cout << res[r][c].c_str() << "\t";
			}
			cout << "\n";
		}
	} catch (const exception &e) {
		cerr << e.what() << endl;
	}
}

// returns "" if invalid
string Database::searchID(int id) {
	try {
		pqxx::work tx(conn());
        pqxx::result code = tx.exec_params("SELECT codename FROM players WHERE id = $1",id);
        
        if (code.empty()) {
			return "";
		}
		return code[0][0].c_str();
	} catch (const exception &e) {
		cerr << e.what() << endl;
		return "";
	}
}

void Database::addPlayerToDatabase(int id, string codename) {
	// check duplicates
	if (searchID(id) == "") {
		try {
			pqxx::work tx(conn());
			tx.exec_params("INSERT INTO players (id, codename) VALUES ($1, $2)", id, codename);
			tx.commit();
			cout << "Successfully added " + codename << endl;
		} catch (const exception &e) {
			cerr << e.what() << endl;
		}
	} else {
		cout << "Player ID " + to_string(id) + " already in use." << endl;
	}
}

void Database::editCodename(int id, string codename) {
	// checks if valid
	if (searchID(id) != "") {
		try {
			pqxx::work tx(conn());
			tx.exec_params("UPDATE players SET codename = $1 WHERE id = $2", codename, id);
			tx.commit();
			cout << "Successfully edited ID: " + to_string(id) + ", new Codename = " + codename << endl;
		} catch (const exception &e) {
			cerr << e.what() << endl;
		}
	}
}

// player functions
int Database::addPlayerToPlayers(int id, string codename, Player::Team team, int equipmentID) {
	// if id doesnt match codename in database
	if (searchID(id) != codename) { 
		return 1;
	}

	// if equipment id is taken
	auto eqChange = players.find(equipmentID);
	if (eqChange != players.end() && eqChange->second.id != id) { 
		return 3;
	}

	// loops through players to remove any duplicates and team changes
	for (auto player: players) {
		if(player.second.id == id) {
			players.erase(player.first);
			break;
		}
	}

	Player temp;
	temp.id = id;
	temp.team = team;
	temp.codename = codename;
	temp.equipmentID = equipmentID;
	players.insert({equipmentID, temp});
	return 0;
}

std::map<int, Player> Database::getPlayers(Player::Team team) {
	std::map<int, Player> roster;
	for (auto player: players) {
		if (player.second.team == team) {
			roster.insert(player);
		}
	}
	return roster;
}

int Database::getTeamScore(Player::Team team) {
	int teamScore = 0;
	for (auto player: players) {
		if (player.second.team == team) {
			teamScore += player.second.score;
		}
	}
	return teamScore;
}

// udp_broadcast lets Database know what happened, then Database handles scoring accordingly 
void Database::handleScoreEvent(int eqID1, int eqID2, bool targetWasBase) {
	// if the bool is true, then eqID2 becomes irrelivant to this function and won't be able to cause issues
	if (targetWasBase) {
		players.at(eqID1).score += 100;
		players.at(eqID1).hasHitBase = true;
		// DEBUG: displays eqID1's score after it gets updated
		//std::cout << "DEBUG: The shooting player with the hardware ID of " << eqID1 << "\'s score is now: " << players.at(eqID1).score << std::endl;
	} else if (friendlyFire(eqID1, eqID2)) {
		// Does not allow for the shooter's points to be deducted to negative values
		if (players.at(eqID1).score >= 10) {
			players.at(eqID1).score = players.at(eqID1).score - 10;
		} else {
			players.at(eqID1).score = 0;
		}
		// DEBUG: displays eqID1's score after it gets updated
		//std::cout << "DEBUG: The shooting player with the hardware ID of " << eqID1 << "\'s score is now: " << players.at(eqID1).score << std::endl;

		// Does not allow for the target's points to be deducted to negative values
		if (players.at(eqID2).score >= 10) {
			players.at(eqID2).score = players.at(eqID2).score - 10;
		} else {
			players.at(eqID2).score = 0;
		}
		// DEBUG: displays eqID2's score after it gets updated
		//std::cout << "DEBUG: The targeted player with the hardware ID of " << eqID2 << "\'s score is now: " << players.at(eqID2).score << std::endl;
	} else {
		players.at(eqID1).score += 10;
		// DEBUG: displays eqID1's score after it gets updated
		//std::cout << "DEBUG: The shooting player with the hardware ID of " << eqID1 << "\'s score is now: " << players.at(eqID1).score << std::endl;
		
		// Does not allow for the target's points to be deducted to negative values
		if (players.at(eqID2).score >= 10) {
			players.at(eqID2).score = players.at(eqID2).score - 10;
		} else {
			players.at(eqID2).score = 0;
		}
		// DEBUG: displays eqID2's score after it gets updated
		//std::cout << "DEBUG: The targeted player with the hardware ID of " << eqID2 << "\'s score is now: " << players.at(eqID2).score << std::endl;
	}
}

bool Database::friendlyFire(int eqID1, int eqID2) {
	if (players.at(eqID1).team == players.at(eqID2).team) {
		return true;
	}
	return false;
}

void Database::clearPlayers(bool red, bool green) {
	for(auto it = players.begin(); it != players.end();) {
		bool isRed = (it -> second.team == Player::Team::RED);
		bool isGreen = (it -> second.team == Player::Team::GREEN);
		
		if ((red && isRed) || (green && isGreen)) {
			it = players.erase(it);
		} else {
			it++;
		}
	}
}

int Database::getNumofPlayers() {
	return players.size();
}

