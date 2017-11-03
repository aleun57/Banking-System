#include "stdafx.h"
#include "Manager.h"


Manager::Manager()
{
}

string Manager::getPassword() {
	return m_password;
}

string Manager::getUsername() {
	return m_username;
}

Manager::~Manager()
{
}
