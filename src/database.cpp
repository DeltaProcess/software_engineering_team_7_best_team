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

bool Database::friendlyFire(int eqID1, int eqID2) {
	if (players.at(eqID1).team == players.at(eqID2).team) {
		return true;
	}
	return false;
}



