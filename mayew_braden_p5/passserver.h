//**********************************************************************************************//
// Braden Mayew                                                                                 //
// COP 4530                                                                                     //
// Project 5 - Hash Tables and its Applications                                                 //
// ------------------------------------- passserver.h ----------------------------------------- //
//**********************************************************************************************//

#ifndef PASSSERVER_H
#define PASSSERVER_H

#include "hashtable.h"
#include<iostream>
#include<string>
#include<cstring>
#include<unistd.h>
#include<crypt.h>
#include<stdlib.h>
#include<stdlib.h>

using namespace std;
using namespace cop4530;

class PassServer  {
	public:
		// Constructor
		PassServer(size_t size = 101);

		// Destructor
		~PassServer();
		
		// Add new username and password. Password passed as plain text, encrypt the password prior to insertion
		bool addUser(std::pair<string, string> & kv);
		
		// Move version of addUser
		bool addUser(std::pair<string, string> && kv);

		// Output contents of passserver
		void dump() const;

		// Delete a user from hashtable based on username 
		bool removeUser(const string & k);

		// Return size of passserver hashtable
		size_t size() const;

		// Checks if user is in the hashtable
		bool find(const string & user) const;

		// Read from files and write to files
		bool load(const char* filename);
		bool write_to_file(const char* filename) const;

		// Allow the user to change their password, but it cannot be the previous password used
		bool changePassword(const pair<string, string> & p, const string & newpassword);	

	private:
		HashTable<string, string> hash_table;
		string encrypt(const string & str);
};
#include "passserver.cpp"
#endif