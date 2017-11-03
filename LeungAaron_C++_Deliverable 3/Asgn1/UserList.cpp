#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <tuple>

#include "UserList.h"
#include "User.h"



using namespace std;

//UserList constructor 
UserList::UserList(string username, string password, int chequing, int savings)
{
	head = new User(username, password, chequing, savings);
	//Add attribute in User called NEXT to keep track of what is the next "User" in the list
}


//Adding Users to the User List, but checks first to see if the username already exists
//return 0 if addding user successful, returns -1 if provided username is found in an existing user
int UserList::AddUser(string username, string password, int chequing, int savings) 
{
	User* user = UsernameSearch(username);

	//If there is no existing user with the provided username, then commence the adding
	if (user == NULL) {
		User* addingUser = new User(username, password, chequing, savings);
		User* current = head;
		while (current) {
			if (current->getNext() == NULL) {
				current->setNext(addingUser);
				return 0;
			}
			current = current->getNext();
		}
	}
	else {
		return -1;
	}
	return 0;
}

int UserList::AddUser(string username, string password, int chequing, int savings,int attempts)
{
	User* user = UsernameSearch(username);

	//If there is no existing user with the provided username, then commence the adding
	if (user == NULL) {
		User* addingUser = new User(username, password, chequing, savings, attempts);
		User* current = head;
		while (current) {
			if (current->getNext() == NULL) {
				current->setNext(addingUser);
				return 0;
			}
			current = current->getNext();
		}
	}
	else {
		return -1;
	}
	return 0;
}

//UsernameSearch -> returns a pointer to a user if the username provided is linked to a user, else return NULL
User* UserList::UsernameSearch(string username) {
	User* current = head;
	while (current) {
		if (current->getUsername() == username) {
			return current;
		}
		current = current->getNext();
	}
	return NULL;
}

//UserLogin -> checks the username/password combination 
//Returns 0 if username and password combination corresponds to an existing user's information
//Else returns -1
int UserList::UserLogin(string username, string password) {
	User* probe = UsernameSearch(username);

	//if a user with the given username does not exist
	if (probe == NULL) {
		return -1;
	}
	//if password does not correspond to the user 
	if (probe->getPassword() != password) {
		return 1;
	}
	else {
		return 0;
	}
}

User* UserList::getHead() {
	return head;
}

//Returns numberOfUsers, total money in chequing accounts, total money in savings accounts, total money in bank.
tuple<int,int,int,int> UserList::stats() {
	User* iterator = getHead();
	int total = 0;
	int chequing = 0;
	int savings = 0;
	int numberOfUsers = 1;
	total += iterator->getChequing() + iterator->getSavings();
	chequing += iterator->getChequing();
	savings += iterator->getSavings();
	while (iterator->getNext()) {
		iterator = iterator->getNext();
		total += iterator->getChequing() + iterator->getSavings();
		chequing += iterator->getChequing();
		savings += iterator->getSavings();
		numberOfUsers += 1;
	}
	return{numberOfUsers, chequing, savings, total};
}

void UserList::saveUsers() {
	User* nextUser = head;
	ofstream myfile;
	myfile.open("users.csv");

	while (nextUser) {
		string entireLine, username, password, chequing, savings, attempts;
		
		username = nextUser->getUsername();
		password = nextUser->getPassword();
		int intChequing = nextUser->getChequing();
		int intSavings = nextUser->getSavings();
		int intAttempts = nextUser->getLoginAttempts();
		attempts = to_string(intAttempts);
		chequing = to_string(intChequing);
		savings = to_string(intSavings);

		entireLine = username + "," + password + "," + chequing + "," + savings + "," + attempts + "\n";
		myfile << entireLine;

		nextUser = nextUser->getNext();
	}
	myfile.close();
}

void UserList::loadUsers() {
	ifstream inf("users.csv");
	if (!inf) {
	}
	else {
		UserList* F = this;
		while (inf.good()) {
			string iUser, iPass, iStrChequing, iStrSavings, iStrAttempts;
			int iChequing, iSavings, iAttempts;
			char delim(',');

			getline(inf, iUser, delim);
			getline(inf, iPass, delim);
			getline(inf, iStrChequing, delim);
			getline(inf, iStrSavings, delim);
			getline(inf, iStrAttempts);

			if (iUser == "") {
				break;
			}
			iChequing = stoi(iStrChequing);
			iSavings = stoi(iStrSavings);
			iAttempts = stoi(iStrAttempts);
			int b;
			b = AddUser(iUser, iPass, iChequing, iSavings,iAttempts);
		}
	}
}


