#pragma once
#include <string>
#include <stack>

using namespace std;
class Manager
{
private:
	string m_username = "manager";
	string m_password = "manager";

public:
	Manager();
	~Manager();
	string getPassword();
	string getUsername();
};

