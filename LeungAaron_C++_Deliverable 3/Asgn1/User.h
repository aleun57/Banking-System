#include <string>
using namespace std;
#ifndef DATE_H
#define DATE_H


class User
{
private:
	User* m_next;
	string m_username;
	string m_password;
	int m_savings;
	int m_chequing;
	int m_loginAttempts;

public:

	int getSavings();
	int getLoginAttempts();
	void failedLogin();
	void lockAccount();
	void unlockAccount();
	int getChequing();
	User(string username, string password, int chequing, int savings);
	User(string username, string password, int chequing, int savings, int attempts);
	void setNext(User * NextUser);
	void close(string account);
	void changeBalance(string chequingOrSavings, int change);
	void setPassword(string newPassword);
	User* getNext();
	string getUsername();
	string getPassword();
};
#endif


