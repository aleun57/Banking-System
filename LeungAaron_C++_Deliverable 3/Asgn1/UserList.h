#include <string>
#include "User.h"
#include <tuple>

using namespace std;
#ifndef USERLIST_H
#define USERLIST_H

class UserList
{
private:
	User* head;
	
	
public:
	UserList(string username, string password, int chequing, int savings);
	int AddUser(string username, string password, int chequing, int savings);
	int AddUser(string username, string password, int chequing, int savings, int attempts);
	User* UsernameSearch(string username);
	int UserLogin(string username, string password);
	User * getHead();
	tuple<int,int,int,int> stats();
	void loadUsers();
	void saveUsers();
};

#endif

