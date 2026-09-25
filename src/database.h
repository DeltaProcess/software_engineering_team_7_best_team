#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <pqxx/pqxx>
#include <string>
using namespace std;
//awesome header file

class Database{
 public:
   //Constructor
   Database();

   //Methods
   void printTable();
   string searchID(int id);
   void addPlayer(int id, string codename);
  
  private:
	pqxx::connection c;

};

#endif
