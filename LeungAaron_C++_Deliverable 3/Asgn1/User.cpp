#include "stdafx.h"
#include <iostream>
#include "user.h"
#include <string>
using namespace std;



User::User(string username, string password, int chequing, int savings)
{
	m_username = username;
	m_password = password;
	m_chequing = chequing;
	m_savings = savings;
	m_next = NULL;
	m_loginAttempts = 3;
}

User::User(string username, string password, int chequing, int savings, int attempts)
{
	m_username = username;
	m_password = password;
	m_chequing = chequing;
	m_savings = savings;
	m_next = NULL;
	m_loginAttempts = attempts;
}

void User::setNext(User* NextUser) {
	m_next = NextUser;
}

void User::close(string account) {
	if (account == "chequing") {
		m_chequing = 0;
	}
	else if (account == "savings") {
		m_savings = 0;
	}
}

void User::changeBalance(string chequingOrSavings, int change) {
	if (chequingOrSavings == "chequing") {
		m_chequing += change;
	}
	if (chequingOrSavings == "savings") {
		m_savings += change;
	}
}

void User::setPassword(string newPassword) {
	m_password = newPassword;
}

User* User::getNext() {
	return m_next;
}

string User::getUsername() {
	return m_username;
}

string User::getPassword() {
	return m_password;
}
int User::getChequing() {
	return m_chequing;
}
int User::getSavings() {
	return m_savings;
}
int User::getLoginAttempts() {
	return m_loginAttempts;
}

void User::failedLogin() {
	m_loginAttempts -= 1;
}

void User::lockAccount() {
	m_loginAttempts = 0;
}

void User::unlockAccount() {
	m_loginAttempts = 3;
}