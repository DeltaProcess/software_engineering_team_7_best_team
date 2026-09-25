#include "database.h"
using namespace std;

Database::Database()
	: c("host=/var/run/postgresql port=5432 dbname=photon target_session_attrs=read-write") //initializes connection c
{
	
	cout << "Successfully connected to: " << c.dbname() << endl;
}


void Database::printTable(){
	try {
		//starts a query
		pqxx::work tx(c);
		
		//gets all columns
        pqxx::result res = tx.exec("SELECT * FROM players");

        //prints column names
        for (pqxx::row_size_type col = 0; col < res.columns(); col++) {
            cout << res.column_name(col) << "\t";
        }
        cout << "\n";

        //prints every row
        for (pqxx::result::size_type r = 0; r < res.size(); r++) {
			for (pqxx::row_size_type c = 0; c < res.columns(); c++) {
				cout << res[r][c].c_str() << "\t";
			}
			cout << "\n";
		}
	}catch (const exception &e) {
		cerr << e.what() << endl;
	}
}

string Database::searchID(int id) { //will return an empty string if not found
	try {
		pqxx::work tx(c);
        pqxx::result code = tx.exec_params("SELECT codename FROM players WHERE id = $1",id);
        
        if (code.empty()){
			return "";
		}
		return code[0][0].c_str();
		
	}catch (const exception &e) {
		cerr << e.what() << endl;
		return "";
	}
	
}

void Database::addPlayer(int id, string codename){ 
	if (searchID(id) == ""){ //no duplicates
		try{
			pqxx::work tx(c);
			tx.exec_params("INSERT INTO players (id, codename) VALUES ($1, $2)", id, codename);
			tx.commit();
			cout << "Successfully added " + codename << endl;
		} catch (const exception &e) {
			cerr << e.what() << endl;
		}
	}else{
		cout << "Player ID " + to_string(id) + " already in use." << endl;
	}
}






