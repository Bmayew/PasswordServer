//**********************************************************************************************//
// Braden Mayew                                                                                 //
// COP 4530                                                                                     //
// Project 5 - Hash Tables and its Applications                                                 //
// ------------------------------------ passserver.cpp ---------------------------------------- //
//**********************************************************************************************//
#include "passserver.h"
#include "hashtable.h"

using namespace std;
using namespace cop4530;

// Constructor
PassServer::PassServer(size_t size)  
{
	hash_table.setSize(size);
	cout << "Server of size " << size << " was created successfully. \n";
}

// Destructor
PassServer::~PassServer() {hash_table.clear();}

// copy version addUser
bool PassServer::addUser(std::pair<string, string> & kv)  
{	string encryptedString = encrypt(kv.second);
	std::pair<string,string> encryptedPair(kv.first, encryptedString);
	if (hash_table.insert(encryptedPair)) {
		cout << "User " << kv.first << " was added successfully. \n";
		return true;
	}
	else {
		//cout << "User " << kv.first << " could not be added successfully. \n";
		return false;
	}
}

// move version addUser
bool PassServer::addUser(std::pair<string, string> && kv)  
{
	// use std::move to move the data not copy
	pair<string, string> tempPair (std::move(kv));
	if (addUser(tempPair)) {return true;}
	else {return false;}
}

bool PassServer::removeUser(const string & k)
{
	if (hash_table.remove(k)) {
		//cout << "User " << k << " was successfully deleted from the table. \n";
		return true;
	}

	else {
		//cout << "User " << k << " could not be deleted from the table. \n";
		return false;
	}
}

bool PassServer::changePassword(const pair<string, string> & p, const string & newpassword)
{

	if (!hash_table.contains(p.first)) {
		cout << "*****Error. User " << p.first << " was not found in the table.\n";
		return false;
	}
	else if (hash_table.updatePassword_Check(p, newpassword)) {
		cout << "*****Error: This password was already in use.\n";
		return false;
	}

	else  {
		string oldPassword = encrypt(p.second);
		string newPassword = encrypt(newpassword);
		pair<string, string> oldPair(p.first, oldPassword);
		if(!hash_table.match(oldPair)) {
			cout << "*****Error: The old password entered is incorrect.\n";
			return false;  
		}
		else  {
			pair<string, string> newPair(p.first, newPassword);
			hash_table.insert(newPair);
			return true;
		}
	}
}	


// Dumps contents of hashtable
void PassServer::dump() const {hash_table.dump();}

// Returns number of users in the Passserver hashtable
size_t PassServer::size() const {return hash_table.getSize();}

// Checks if user is in the hashtable
bool PassServer::find(const string & user) const
{
	pair<string, string> tempPair;
	tempPair.first = user;

	if (hash_table.contains(tempPair.first)) {
		cout << "Success. User " << user << " located in table.\n";
		return true;
	}
	else {
		//cout << "Error. User " << user << " could not be located in table.\n";
		return false;
	}
}

bool PassServer::load(const char* filename)
{
	return hash_table.load(filename);
}

bool PassServer::write_to_file(const char* filename) const
{
	return hash_table.write_to_file(filename);
}

string PassServer::encrypt(const string & str) 
{
	char salt[] = "$1$########";
    char * encryptedPassword = new char [100];
    strcpy (encryptedPassword, crypt(str.c_str(), salt));
	return encryptedPassword;
}



