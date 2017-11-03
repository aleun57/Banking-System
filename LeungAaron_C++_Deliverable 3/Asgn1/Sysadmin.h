#pragma once
#include <string>
#include <vector>

using namespace std;
class Sysadmin
{
private: 
	bool m_trace;
	string m_username = "sysadmin";
	string m_password = "sysadmin";
	vector<string> theStack;
public:
	Sysadmin();
	void push(string str);
	vector<string> getVector();
	void turnOffTrace();
	void turnOnTrace();
	bool isTraceOn();
	string getUsername();
	string getPassword();
	~Sysadmin();
};

