//**********************************************************************************************//
// Braden Mayew                                                                                 //
// COP 4530                                                                                     //
// Project 5 - Hash Tables and its Applications                                                 //
// -------------------------------------- sproj5.cpp ------------------------------------------ //
//**********************************************************************************************//

#include<iostream>
#include<utility>
#include <termios.h>

#include "passserver.h"
#include "hashtable.h"

using namespace std;

// provided menu function
void Menu()  
{
  cout << "\n\n";
  cout << "l - Load From File" << endl;
  cout << "a - Add User" << endl;
  cout << "r - Remove User" << endl;
  cout << "c - Change User Password" << endl;
  cout << "f - Find User" << endl;
  cout << "d - Dump HashTable" << endl;
  cout << "s - HashTable Size" << endl;
  cout << "w - Write to Password File" << endl;
  cout << "x - Exit program" << endl;
  cout << "\nEnter choice : ";
}

void hideStrokes()
{
    termios t;
    tcgetattr(STDIN_FILENO, &t);
    // disable echo 
    t.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

void showStrokes()
{
   termios t;
   tcgetattr(STDIN_FILENO, &t);
   // enable echo
   t.c_lflag |= ECHO;
   tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

int main() 
{
	size_t serverSize = 0;
	string menuInput;
	char newInput;
	string userName;
	string password;
	
	cout << "\nPlease enter the desired server size: ";
	cin >> serverSize;
	if (serverSize <= 0) {
		cout << "Unable to create a table of this size.\n";
		serverSize = 101; // default server size to 101
	}
	PassServer server(serverSize);

	do {
		showStrokes();
		Menu();
		// Error checking for input like "adam" when we only want the 'a'
		newInput = ' ';
  		cin >> menuInput;
  		if (menuInput.size() != 1) {
   			newInput = 'z';
  		}
  		else {newInput = menuInput[0];}


		if (newInput == 'l') {
			char inFile[50];
			cout << "Enter a filename to load: ";
			cin >> inFile;
			while (!server.load(inFile)) {
				cout << "Invalid file name. Enter new file name: ";
				cin >> inFile;
			}
			cout << "The file " << inFile << " was loaded successfully\n"; 
		}

		else if (newInput == 'a') {	
			std::pair<string, string> ID;
			cout << "Enter desired username: ";
			cin >> userName;
			ID.first = userName;

			hideStrokes();
			cout << "Enter desired password: ";
			cin >> password;
			ID.second = password;
			if (!server.addUser(ID)) {
				cout << "*****Error: Unable to create profile: " << ID.first << endl;
			}
		}

		else if (newInput == 'r') {
			std::pair<string, string> ID;
			cout << "Enter the username you wish to delete: ";
			cin >> userName;
			ID.first = userName;
			if (!server.removeUser(ID.first)) {
				cout << "*****Error: Unable to delete username: " << ID.first << endl;
			}
		}

		else if (newInput == 'c') {
			string oldPassword;
			cout << "Enter your username: ";
			cin >> userName;

			hideStrokes();
			cout << "Enter password: ";
			cin >> oldPassword;
			cout << endl;
			cout << "Enter the new password: ";
			cin >> password;
			std::pair<string, string> ID (userName, oldPassword);
			if (server.changePassword(ID, password)) {
				cout << "Password changed successfully.\n";
			}
			else {
				cout << "*****Error: Password update failure.\n";
			}
		}

		else if	(newInput == 'f') {
			cout << "Enter the username you wish to find: ";
			cin >> userName;
			if (!server.find(userName)) {
				cout << "*****Error: Unable to find username: " << userName << endl; 
			}

		}

		else if (newInput == 'd') {
			server.dump();
		}

		else if (newInput == 's') {
			cout << "The number of User(s) in the PassServer is " << server.size() << endl;
		}

		else if	(newInput == 'w') {
			char outFile[50];
			cout << "Enter a filename to write to: ";
			cin >> outFile;
			if (!server.write_to_file(outFile)) {
				cout << "*****Error: Unable to write to file. Enter new filename: ";
			}
			else {
				cout << "The file " << outFile << " was successfully written to\n"; 
			}
		}

		else {
			if (newInput != 'x')
				cout << "*****Error: Not a valid Menu option...Enter a valid choice: ";
		}

	} while (newInput != 'x');

	return 0;	
} 