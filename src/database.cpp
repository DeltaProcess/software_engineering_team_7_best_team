#include <iostream>
#include <pqxx/pqxx>
#include <string>

int init() {
	try {
		//connects to the database
		pqxx::connection c("host=/var/run/postgresql port=5432 dbname=photon target_session_attrs=read-write");
		std::cout << "Successfully connected to: " << c.dbname() << std::endl;	
		
		//starts a query
		pqxx::work tx(c);
		
		//gets all columns
        pqxx::result res = tx.exec("SELECT * FROM players");

        //prints column names
        for (pqxx::row_size_type col = 0; col < res.columns(); col++) {
            std::cout << res.column_name(col) << "\t";
        }
        std::cout << "\n";

        //prints every row
        for (pqxx::result::size_type r = 0; r < res.size(); r++) {
			for (pqxx::row_size_type c = 0; c < res.columns(); c++) {
				std::cout << res[r][c].c_str() << "\t";
			}
			std::cout << "\n";
		}
        
        // tx.commit(); if writing to database, need this line
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	return 0;
}
