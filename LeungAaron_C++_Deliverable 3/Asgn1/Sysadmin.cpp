#include "stdafx.h"
#include "Sysadmin.h"
#include <vector>


using namespace std;
Sysadmin::Sysadmin()
{
	m_trace = false;
}

void Sysadmin::push(string str) {
	theStack.push_back(str);
}

vector<string> Sysadmin::getVector() {
	return theStack;
}

void Sysadmin::turnOffTrace()
{
	m_trace = false;
}

void Sysadmin::turnOnTrace() {
	m_trace = true;
}

bool Sysadmin::isTraceOn() {
	return m_trace;
}

string Sysadmin::getUsername() {
	return m_username;
}

string Sysadmin::getPassword() {
	return m_password;
}


Sysadmin::~Sysadmin()
{
}
