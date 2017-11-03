// Asgn1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <tuple>
#include <ctime>
#include <chrono>
#include <fstream>
#include <iomanip>

#include "User.h"
#include "UserList.h"
#include "Sysadmin.h"
#include "Manager.h"
#include <stack>
#include <cstdlib>

//Method to clear all the text from the screen
void SetStdinEcho(bool enable = true)
{
#ifdef WIN32
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);

	if (!enable)
		mode &= ~ENABLE_ECHO_INPUT;
	else
		mode |= ENABLE_ECHO_INPUT;

	SetConsoleMode(hStdin, mode);

#else
	struct termios tty;
	tcgetattr(STDIN_FILENO, &tty);
	if (!enable)
		tty.c_lflag &= ~ECHO;
	else
		tty.c_lflag |= ECHO;

	(void)tcsetattr(STDIN_FILENO, TCSANOW, &tty);
#endif
}

using namespace std;

//Method to check whether input has no Commas
bool noCommas(string str) {
	char comma = ',';
	for (string::size_type i = 0; i < str.size(); ++i) {
		if (str[i] == comma) {
			return false;
		}
	}
	return true;
}

//Method to check if all characters in string are numbers
bool allNumbers(string str) {
	for (string::size_type i = 0; i < str.size(); ++i) {
		if ((int)str[i] > 57 || (int)str[i] < 48) {
			return false;
		}
	}
	return true;
}

//System Admin Menu Options
void systemAdminMenu(UserList test, Sysadmin* sysadmin, Manager manager) {
	string input = "-1";
	system("CLS");
	while (input != "0") {

		cout << "Please input a number that corresponds to the options listed below" << endl;
		cout << "0. Logout of your account" << endl;
		cout << "1. Turn ON the system trace" << endl;
		cout << "2. Turn OFF the system trace" << endl;
		getline(cin, input);
		if (input == "0") {}
		//Turning on the Trace
		else if (input == "1") {
			sysadmin->turnOnTrace();
			cout << "Trace successfully turned ON" << endl << endl;
		}
		//Turning off the Trace 
		else if (input == "2") {
			sysadmin->turnOffTrace();
			cout << "Trace successfully turned OFF" << endl << endl;
		}
		else {
			cout << "Please input a number" << endl << endl;
		}
	}
}

//Manager Menu Options
void managerMenu(UserList test, Sysadmin sysadmin, Manager manager) {
	string input = "-1";
	system("CLS");
	while (input != "0") {
		cout << "Please input a number that corresponds to the options listed below" << endl;
		cout << "0. Logout of your Account" << endl;
		cout << "1. Create a User Account" << endl;
		cout << "2. View/Open/Close a User's Chequing or Savings Account" << endl;
		cout << "3. View entire bank statistics" << endl;
		cout << "4. Unlock/Lock a User's Account" << endl;
		getline(cin, input);

		//Logout of Account 
		if (input == "0") {}

		//Creating an Account 
		else if (input == "1") {
			string nUsername, nPassword;
			int nChequing, nSavings;

			//Only Restriction for username and password is no comma value
			cout << "Please enter no commas in username and password" << endl;
			cout << "New User's Username: " << endl;
			getline(cin, nUsername);
			while (noCommas(nUsername) == false) {
				cout << "Really, no commas please." << endl << "New user's Username: ";
				getline(cin, nUsername);
			}

			cout << "New User's Password:" << endl;
			getline(cin, nPassword);
			while (noCommas(nPassword) == false) {
				cout << "Really, no commas please." << endl << "New User's Password: ";
				getline(cin, nPassword);
			}

			//Creation of Chequing account, ask manager if want to enable before entering initializing amount
			string yesOrNo = "a";
			string money;
			while (yesOrNo != "yes" && yesOrNo != "no") {
				cout << "Enable Chequing account? (yes/no) ";
				getline(cin, yesOrNo);
				if (yesOrNo == "no") {
					nChequing = 0;}
				else if (yesOrNo == "yes") {
					cout << "Input initial dollar amount in Chequing account: ";
					getline(cin, money);
					while (allNumbers(money) == false) {
						cout << "Please input only numbers [0-9]" << endl << "Input initial dollar amount in Chequing account: ";
						getline(cin, money);
					}
					nChequing = stoi(money);
				}
				else { cout << "Please input either 'yes' or 'no'" << endl; }
			}

			//Savings Account stuff
			yesOrNo = "a";
			while (yesOrNo != "yes" && yesOrNo != "no") {
				cout << "Enable Savings account? (yes/no) ";
				getline(cin, yesOrNo);
				if (yesOrNo == "no") {
					nSavings = 0;}
				else if (yesOrNo == "yes") {
					string money = "a";
					while (allNumbers(money) == false) {
						cout << "Input initial dollar amount in Savings account: ";
						getline(cin, money);
						while (allNumbers(money) == false) {
							cout << "Please input only numbers [0-9]" << endl << "Input initial dollar amount in Savings account: ";
							getline(cin, money);
						}
						nSavings = stoi(money);
					}
				}
				else { cout << "Please input either 'yes' or 'no'" << endl; }
			}

			int addingUser = test.AddUser(nUsername, nPassword, nChequing, nSavings);
			if (addingUser == -1) {
				cout << "ERROR: User with the same Username already exists. Proposed User was not added." << endl << endl;
			}
			else {
				cout << "Successfully created User " + nUsername << endl << endl;
			}
		}

		// View/Open/Close a User's Chequing or Savings Account
		else if (input == "2") {
			string aUsername;
			cout << "User's Username:" << endl;
			getline(cin, aUsername);
			User* theUser = test.UsernameSearch(aUsername);
			bool emptyChequing = false;
			bool emptySavings = false;
			if (theUser == NULL) {
				cout << "User with that username does not exist" << endl;
			}
			else {
				//Display info about the account
				string strChequing, strSavings;
				if (theUser->getChequing() == 0) {
					emptyChequing = true;
					strChequing = "ACCOUNT NOT OPENED";
				}
				else { strChequing = to_string(theUser->getChequing()); }
				if (theUser->getSavings() == 0) {
					emptySavings = true;
					strSavings = "ACCOUNT NOT OPENED";
				}
				else { strSavings = to_string(theUser->getSavings()); }
				if (theUser->getLoginAttempts() == 0) { cout << "Account Status: LOCKED" << endl; }
				else { cout << "Account Status: UNLOCKED" << endl; }
				cout << "Chequing Balance: " + strChequing << endl;
				cout << "Savings Balance: " + strSavings << endl;
			}
			string yesOrNo= "a";
			string strMoney = "a";
			//Open/Closing chequing or savings account 
			while (yesOrNo != "yes" && yesOrNo != "no") {
				cout << "Open/Close Accounts? (yes/no) ";
				getline(cin, yesOrNo);
				if (yesOrNo == "yes") {
					//Scenario when chequing is CLOSED and savings is OPEN
					if (emptyChequing == true && emptySavings == false) {
						string ff = "a";
						while (ff != "1" && ff != "2") {
							cout << endl << "1. Open Chequing Account" << endl;
							cout << "2. Close Savings Account" << endl;
							getline(cin, ff);
							if (ff == "1") {
								theUser->changeBalance("chequing", 1);
								cout << endl << "Successfully opened Chequing" << endl << endl;
							}
							else if (ff == "2") {
								theUser->close("savings");
								cout << endl << "Successfully closed Savings" << endl << endl;
							}
							else { cout << endl << "Please input either '1' or '2'" << endl; }
						}
					}
					//Scenario when chequing is OPEN and savings is CLOSED
					else if (emptyChequing == false && emptySavings == true) {
						string ff = "a";
						while (ff != "1" && ff != "2") {
							cout << endl << "1. Close Chequing Account" << endl;
							cout << "2. Open Savings Account" << endl;
							getline(cin, ff);
							if (ff == "1") {
								theUser->close("chequing");
								cout << endl << "Successfully closed Chequing" << endl << endl;
							}
							else if (ff == "2") {
								theUser->changeBalance("savings",1);
								cout << endl << "Successfully opened Savings" << endl << endl;
							}
							else { cout << endl << "Please input either '1' or '2'" << endl; }
						
						}
					}
					//Scenario when chequing is CLOSED and savings is CLOSED
					else if (emptyChequing == true && emptySavings == true) {
						string ff = "a";
						while (ff != "1" && ff != "2") {
							cout << endl << "1. Open Chequing Account" << endl;
							cout << "2. Open Savings Account" << endl;
							getline(cin, ff);
							if (ff == "1") {
								theUser->changeBalance("chequing", 1);
								cout << endl << "Successfully opened Chequing" << endl << endl;
							}
							else if (ff == "2") {
								theUser->changeBalance("savings", 1);
								cout << endl << "Successfully opened Savings" << endl << endl;
							}
							else { cout << endl << "Please input either '1' or '2'" << endl; }
						}
					}
					//Scenario when chequing is CLOSED and savings is CLOSED
					else if (emptyChequing == false && emptySavings == false) {
						string ff = "a";
						while (ff != "1" && ff != "2") {
							cout << endl << "1. Close Chequing Account" << endl;
							cout << "2. Close Savings Accout" << endl;
							getline(cin, ff);
							if (ff == "1") {
								theUser->close("chequing");
								cout << endl << "Successfully closed Chequing" << endl << endl;
							}
							else if (ff == "2") {
								theUser->close("savings");
								cout << endl << "Successfully closed Savings" << endl << endl;
							}
							else { cout << endl << "Please input either '1' or '2'" << endl; }
						}
					}
				}
				else if (yesOrNo == "no") {}
				else { cout << endl << "Please input either 'yes' or 'no'" << endl; }
			}
		}

		//View the entire Bank's Statistics (Amount of money in bank and # of Users)
		else if (input == "3") {
			tuple<int, int, int, int> stat = test.stats();
			cout << endl << "Number of Users in the bank: " + to_string(get<0>(stat)) << endl;
			cout << "All Chequing money in bank: " + to_string(get<1>(stat)) << endl;
			cout << "All Savings money in bank: " + to_string(get<2>(stat)) << endl;
			cout << "Total money in the bank: " + to_string(get<3>(stat)) << endl << endl;
		}
		//Lock/Unlock account
		else if (input == "4") {
			string aUsername;
			cout << "User's Username:" << endl;
			getline(cin, aUsername);
			User* theUser = test.UsernameSearch(aUsername);
			if (theUser == NULL) {
				cout << "User with that username does not exist" << endl;
			}
			else {
				string yesOrNo = "A";
				if (theUser->getLoginAttempts() == 0) {
					while (yesOrNo != "yes" && yesOrNo != "no") {
						cout << "Account Status: LOCKED" << endl << "Unlock the account? (yes/no)" << endl;
						getline(cin, yesOrNo);
						if (yesOrNo == "yes") {
							theUser->unlockAccount();
							cout << "Account " + theUser->getUsername() + " has been UNLOCKED" << endl;
						}
						else if (yesOrNo == "no") { cout << "Unlocking cancelled" << endl; }
						else { cout << "Please input 'yes' or 'no'" << endl; }
					}
				}
				else {
					while (yesOrNo != "yes" && yesOrNo != "no") {
						cout << "Accout Status: UNLOCKED" << endl << "Lock the account? (yes/no)" << endl;
						getline(cin, yesOrNo);
						if (yesOrNo == "yes") {
							theUser->lockAccount();
							cout << "Account " + theUser->getUsername() + " has been LOCKED" << endl;
						}
						else if (yesOrNo == "no") { cout << "Locking cancelled" << endl; }
						else { cout << "Please input 'yes' or 'no'" << endl; }
					}
				}
			}
		}
		else {
			cout << "Please input a number" << endl << endl;
		}
	}
}

//User Menu 
void userMenu(UserList test, Sysadmin* sysadmin, Manager maanger, string username) {
	string input = "-1";
	system("CLS");
	User* theUser = test.UsernameSearch(username);
	int chequing, savings;

	time_t result = time(NULL);
	char str[26];
	ctime_s(str, sizeof str, &result);
	if (sysadmin->isTraceOn() == true) { sysadmin->push("User " + username + " logged in at " + str); }

	while (input != "0") {
		cout << "Please input a number that corresponds to the options listed below" << endl;
		cout << "0. Logout of account" << endl;
		cout << "1. Transfer funds" << endl;
		cout << "2. Deposit funds" << endl;
		cout << "3. Withdraw funds" << endl;
		cout << "4. Change Password" << endl;
		cout << "5. View Interest Calculator" << endl << endl;
		cout << "User " + theUser->getUsername() + " Account Balances" << endl;
		bool emptyChequing = false;
		bool emptySavings = false;
		if (theUser->getChequing() == 0) {
			cout << "Chequing Account: NOT OPENED" << endl;
			emptyChequing = true;}
		else { cout << "Chequing Account: " + to_string(theUser->getChequing()) << endl; }
		if (theUser->getSavings() == 0) {
			cout << "Savings Account: NOT OPENED" << endl;
			emptySavings = true;}
		else { cout << "Savings Account: " + to_string(theUser->getSavings()) << endl; }

		getline(cin, input);
		//Transfer Funds
		if (input == "1") {
			//If User does not have two accounts
			if (emptySavings == true || emptyChequing == true) {
				cout << endl << "Unable to transfer between accounts because you only have one account open" << endl << endl;
			}
			else {
				string cOrS = "";
				while (cOrS != "chequing" && cOrS != "savings") {
					cout << "Select the account you want to transfer FROM (chequing/savings): ";
					getline(cin, cOrS);
					string money = "a";
					int intMoney;
					//Select Dollar Amount they want to transfer
					if (cOrS == "chequing" || cOrS == "savings") {
						while (allNumbers(money) == false || allNumbers(money) == true && money == "0") {
							cout << "Enter the dollar amount you want to transfer: ";
							getline(cin, money); 
							//If they select Chequing to Transfer FROM
							if (allNumbers(money) == true && cOrS == "chequing" && money != "0") {
								intMoney = stoi(money);
								chequing = theUser->getChequing();
								//Actually changing account balances
								if (chequing < 1000 && (chequing - intMoney) > 0 || chequing > 1000 && (chequing - intMoney) > 1000) {
									theUser->changeBalance("chequing", intMoney * -1);
									theUser->changeBalance("savings", intMoney);
									cout << endl << "Transfer of $" + to_string(intMoney) + " from Chequing to Savings made" << endl;
									if (sysadmin->isTraceOn() == true) { sysadmin->push("Tranferred $" + to_string(intMoney) + " from chequing to savings"); }
								}
								else if (chequing >= 1000 && (chequing - intMoney) < 1000 && (chequing - intMoney - 2) > 0) {
									string yesOrNo = "a";
									while (yesOrNo != "yes" && yesOrNo != "no") {
										cout << "Transferring this amount will drop the chequing account to a level below $1000" << endl;
										cout << "A $2.00 levy will be applied if this transaction goes through" << endl;
										cout << "Continue through (yes/no): ";
										getline(cin, yesOrNo);
										if (yesOrNo == "yes") {
											//Actually changing account balances
											theUser->changeBalance("chequing", (intMoney * -1) - 2);
											theUser->changeBalance("savings", intMoney);
											cout << endl << "Transfer of $" + to_string(intMoney) + " from Chequing to Savings made" << endl;
											if (sysadmin->isTraceOn() == true) { sysadmin->push("Tranferred $" + to_string(intMoney) + " from chequing to savings"); }
										}
										else if (yesOrNo == "no") {
											cout << endl << "Transaction cancelled" << endl;
										}
										else { cout << endl << "Please input 'yes' or 'no'" << endl; }
									}
								}
								else {cout << endl << "ERROR: Insufficient funds in the account you want to transfer from " << endl << endl;}
							}
							//If they select Savings to Transfer FROM
							else if (allNumbers(money) == true && cOrS == "savings" && money != "0") {
								intMoney = stoi(money);
								savings = theUser->getSavings();
								//Actually changing account balances
								if (savings < 1000 && (savings - intMoney) > 0 || savings > 1000 && (savings - intMoney) > 1000) {
									theUser->changeBalance("savings", intMoney * -1);
									theUser->changeBalance("chequing", intMoney);
									cout << endl << "Transfer of $" + to_string(intMoney) + " from Savings to Chequing made" << endl;
									if (sysadmin->isTraceOn() == true) { sysadmin->push("Tranferred $" + to_string(intMoney) + " from savings to chequing"); }
								}
								else if (savings >= 1000 && (savings - intMoney) < 1000 && (savings - intMoney - 2) > 0) {
									string yesOrNo = "a";
									while (yesOrNo != "yes" && yesOrNo != "no") {
										cout << "Transferring this amount will drop the savings account to a level below $1000" << endl;
										cout << "A $2.00 levy will be applied if this transaction goes through" << endl;
										cout << "Continue through (yes/no): ";
										getline(cin, yesOrNo);
										if (yesOrNo == "yes") {
											//Actually changing account balances
											theUser->changeBalance("savings", (intMoney * -1) - 2);
											theUser->changeBalance("chequing", intMoney);
											cout << endl << "Transfer of $" + to_string(intMoney) + " from Savings to Chequing made" << endl;
											if (sysadmin->isTraceOn() == true) { sysadmin->push("Tranferred $" + to_string(intMoney) + " from savings to chequing"); }
										}
										else if (yesOrNo == "no") {cout << endl << "Transaction cancelled" << endl << endl;}
										else { cout << endl << "Please input 'yes' or 'no'" << endl; }
									}
								}
								else { cout << endl << "ERROR: Insufficient funds in the account you want to transfer from " << endl << endl; }
							}
							else { cout << endl << "Please input only numbers and not zero" << endl; }
						}
					}
					else {cout << endl << "Please input either 'chequing' or 'savings'" << endl;}
				}
			}
		}
		//Deposit Funds
		else if (input == "2") {
			string cOrS = "a";
			//Select Chequing or Savings
			while (cOrS != "chequing" && cOrS != "savings") {
				cout << endl << "Select the account you want to Deposit to (chequing/savings): ";
				getline(cin, cOrS);
				//Check whether they have access to the selected account
				if (cOrS == "chequing" || cOrS == "savings") {
					if (cOrS == "chequing" && emptyChequing == false || cOrS == "savings" && emptySavings == false) {
						string money = "a";
						//Check whether input is a valid dollar amount
						while (allNumbers(money) == false) {
							cout << "Input the amount of money you are depositing: ";
							getline(cin, money);
							//Actually deposit money
							if (allNumbers(money) == true && cOrS == "chequing") {
								theUser->changeBalance("chequing", stoi(money));
								cout << endl << "Successfully deposited $" + money + "into Chequing account" << endl << endl ;
								if (sysadmin->isTraceOn() == true) { sysadmin->push("Deposited $" + money + " to Chequing account"); }
							}
							else if (allNumbers(money) == true && cOrS == "savings") {
								theUser->changeBalance("savings", stoi(money));
								cout << endl << "Successfully deposited $" + money + " into Savings account" << endl << endl;
								if (sysadmin->isTraceOn() == true) { sysadmin->push("Deposited $" + money + " to Savings account"); }
							}
							else { cout << endl << "Please input only numbers" << endl; }
						}
					}
					else { cout << endl << "You do not have access to the account you have selected" << endl;
					cout << "Please talk with your branch manager if you wish to activate the account" << endl;}
				}
				else { cout << endl << "Please input 'chequing' or 'savings'" << endl; }
			}
		}
		//Withdraw funds
		else if (input == "3") {
			string cOrS = "a";
			//Select Chequing or Savings
			while (cOrS != "chequing" && cOrS != "savings") {
				cout << endl << "Select the account you want to withdraw from (chequing/savings): ";
				getline(cin, cOrS);
				//Check whether they have access to the selected account
				if (cOrS == "chequing" || cOrS == "savings") {
					if (cOrS == "chequing" && emptyChequing == false || cOrS == "savings" && emptySavings == false) {
						string money = "a";
						//Check whether input is a valid dollar amount
						while (allNumbers(money) == false) {
							cout << "Input the amount of money you are withdrawing: ";
							getline(cin, money);
							int intMoney;
							if (allNumbers(money) == true) {
								if (cOrS == "chequing") {
									intMoney = stoi(money);
									//Withdrawing money
									if (theUser->getChequing() < 1000 && (theUser->getChequing() - intMoney) > 0 || theUser->getChequing() - intMoney > 1000) {
										theUser->changeBalance("chequing", intMoney * -1);
										cout << "Successfully withdrew $" + money << endl << endl;
										if (sysadmin->isTraceOn() == true) { sysadmin->push("Withdrew $" + money + " from Chequing account"); }
									}
									else if (theUser->getChequing() >= 1000 && (theUser->getChequing() - intMoney - 2) > 0 && (theUser->getChequing() - intMoney) < 1000) {
										string yesOrNo = "a";
										while (yesOrNo != "yes" && yesOrNo != "no") {
											cout << "Withdrawing this amount will drop the savings account to a level below $1000" << endl;
											cout << "A $2.00 levy will be applied if this transaction goes through" << endl;
											cout << "Continue through (yes/no): ";
											getline(cin, yesOrNo);
											if (yesOrNo == "yes") {
												theUser->changeBalance("chequing", (intMoney * -1) - 2);
												cout << "Successfully withdrew $" + money << endl << endl;
												if (sysadmin->isTraceOn() == true) { sysadmin->push("Withdrew $" + money + " from Chequing account"); }
											}
											else if (yesOrNo == "no") { cout << "Transaction was cancelled" << endl << endl; }
											else { cout << "Please input either 'yes' or 'no'" << endl; }
										}
									}
									else { cout << "Insufficient funds for the transaction" << endl << endl; }
								}
								else if (cOrS == "savings") {
									intMoney = stoi(money);
									//Withdrawing money
									if (theUser->getSavings() < 1000 && (theUser->getSavings() - intMoney) > 0 || theUser->getSavings() - intMoney > 1000) {
										theUser->changeBalance("savings", intMoney * -1);
										cout << "Successfully withdrew $" + money << endl << endl;
										if (sysadmin->isTraceOn() == true) { sysadmin->push("Withdrew $" + money + " from Savings account"); }
									}
									else if (theUser->getSavings() >= 1000 && (theUser->getSavings() - intMoney - 2) > 0 && (theUser->getSavings() - intMoney) < 1000) {
										string yesOrNo = "a";
										while (yesOrNo != "yes" && yesOrNo != "no") {
											cout << "Withdrawing this amount will drop the savings account to a level below $1000" << endl;
											cout << "A $2.00 levy will be applied if this transaction goes through" << endl;
											cout << "Continue through (yes/no): ";
											getline(cin, yesOrNo);
											if (yesOrNo == "yes") {
												theUser->changeBalance("savings", (intMoney * -1) - 2);
												cout << "Successfully withdrew $" + money << endl << endl;
												if (sysadmin->isTraceOn() == true) { sysadmin->push("Withdrew $" + money + " from Savings account"); }
											}
											else if (yesOrNo == "no") { cout << "Transaction was cancelled" << endl << endl; }
											else { cout << "Please input either 'yes' or 'no'" << endl; }
										}
									}
									else { cout << "Insufficient funds for the transaction" << endl << endl; }
								}
							}
							else { cout << endl << "Please input a number" << endl; }
						}
					}
					else {cout << endl << "You do not have access to the account you have selected" << endl;
						cout << "Please talk with your branch manager if you wish to activate the account" << endl;}
				}
				else { cout << endl << "Please input either 'chequing' or 'savings'" << endl; }
			}
		}
		//Change Password
		else if (input == "4") {
			string newPassword = ",";
			while (noCommas(newPassword) == false) {
				cout << "Type in your new password: " << endl;
				getline(cin, newPassword);
				if (noCommas(newPassword) == true) {
					string newPassword2 = ",";
					while (noCommas(newPassword2) == false) {
						cout << "Re-type your new password: " << endl;
						getline(cin, newPassword2);
						if (noCommas(newPassword2) == true && newPassword == newPassword2) {
							theUser->setPassword(newPassword);
							cout << "Successfully changed password" << endl << endl;
							if (sysadmin->isTraceOn() == true) { sysadmin->push("Password has been changed to " + newPassword2); }
						}
						else if (noCommas(newPassword) == true && newPassword != newPassword2) {
							cout << "The second entered password did not match the first entered password" << endl << "Please try again." << endl;
						}
					}
				}
				else { cout << "Please use a password that does not have commas" << endl; }
			}
		}
		//Interest Calculator 
		else if (input == "5") {
			string intRate = "a";
			while (allNumbers(intRate) == false) {
				cout << "Input a yearly interest rate (%): ";
				getline(cin, intRate); 
				if (allNumbers(intRate) == true) {
					float total = theUser->getChequing() + theUser->getSavings();
					float rate = stoi(intRate) * 0.01;
					cout <<  fixed;
					cout << endl << "Total dollar amount in account(s): $" << setprecision(0) << total;
					cout << endl << "At " + intRate + "% a year:" << endl;
					cout << endl << "1 Month: $" << setprecision(0) << (total * (1 + (rate / 12)));
					cout << endl << "2 Months: $" << setprecision(0) << (total * (1 + (rate / 6)));
					cout << endl << "4 Months: $" << setprecision(0) << (total * (1 + (rate / 3)));
					cout << endl << "6 Months: $" << setprecision(0) << (total * (1 + (rate / 2)));
					cout << endl << "12 Months: $" << setprecision(0) << (total * ((1 + rate))) << endl << endl;
				}
				else {
					cout << "Please input a number" << endl;
				}
			}
		}
		else {cout << "Please input a number" << endl;}
	}
}


//Main Menu Options
void mainMenu(UserList test, Sysadmin sysadmin, Manager manager) {
	bool loop = false;
	//Login Process
	while (loop == false) {
		system("CLS");
		cout << "Welcome to the Leung Bank!" << endl;
		cout << "Please enter your username or input EXIT to exit the bank: ";
		string enteredUsername;
		SetStdinEcho(true);
		getline(cin, enteredUsername);

		//Saving the UserList and the Trace if applicable.
		if (enteredUsername == "EXIT") {
			test.saveUsers();
			if (sysadmin.isTraceOn() == true) {
				time_t result = time(NULL);

				struct tm * now = localtime(&result);
				char dateAndTime[80];
				strftime(dateAndTime, 80, "%Y-%m-%d %I-%M-%S", now);
				string placeHolder = dateAndTime;
				//ofstream txtFile("Trace.txt");
				ofstream txtFile;
				txtFile.open(placeHolder + ".txt");

				vector<string> stack = sysadmin.getVector();
				for (vector<string>::iterator it = stack.begin(); it != stack.end(); ++it) {
					txtFile << *it + '\n';
				}
				txtFile.close();
			}
			exit(0);
		}

		cout << "Enter your password: ";
		string enteredPassword;
		SetStdinEcho(false);
		getline(cin, enteredPassword);

		system("CLS");
		int loginValue = test.UserLogin(enteredUsername, enteredPassword);
		User* theUser = test.UsernameSearch(enteredUsername);
		Sysadmin* sys = &sysadmin;
	
		//System Admin Login
		if (enteredUsername == sysadmin.getUsername() && enteredPassword == sysadmin.getPassword()) {
			systemAdminMenu(test, sys, manager);}

		//Manager Login
		else if (enteredUsername == manager.getUsername() && enteredPassword == manager.getPassword()) {
			managerMenu(test, sysadmin, manager);}
		
		//Correct User Login 
		else if (loginValue == 0 && theUser->getLoginAttempts() != 0) {
			theUser->unlockAccount();
			userMenu(test, sys, manager, enteredUsername);
		}
		//Correct User Login but no more login attempts
		else if (loginValue == 0 && theUser->getLoginAttempts() == 0) {
			cout << "Your account has now been locked due to many incorrect password attempts" << endl
				<< "Speak with your branch manager to unlock your account" << endl
				<< "Press ENTER to login to a different account" << endl;
			string f;
			getline(cin, f);
		}
		//Incorrect User Login and decrement login attempts
		else if (theUser && theUser->getLoginAttempts() != 0) {
			theUser->failedLogin();
			cout << "The password for the selected account is incorrect" << endl
				<< "There are " + to_string(theUser->getLoginAttempts()) + " attempts left before account locks"
				<< endl << "Press the ENTER key to try again";
			string f;
			getline(cin, f);
		}
		else {cout << "Incorrect username and password combination" << endl << "Press ENTER to try again.";
		string f;
		getline(cin, f);
		}
	}
}

int main()
{//Initialization, 
	string te = "YOLO";
	UserList test = UserList(te, te, 1000, 1000);
	test.loadUsers();
	Sysadmin sysadmin;
	Manager manager;

	mainMenu(test, sysadmin, manager);
	return 0;
}

