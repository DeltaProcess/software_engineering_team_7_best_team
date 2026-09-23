#include <iostream>
#include <pqxx/pqxx>
#include <string>
using namespace std;
int main() {
	try {
		//connects to the database
		pqxx::connection c("user=student password=student host=/var/run/postgresql port=5432 dbname=photon target_session_attrs=read-write");
		cout << "Successfully connected to: " << c.dbname() << endl;	
		
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
        
       // tx.commit(); if writing to database, need this line
    
	}catch (const exception &e) {
		cerr << e.what() << endl;
		return 1;
	}
	
}
