#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "user.h"

//class UserManager  can be used to solve the following use cases:
// List all users 
// Find  user 
// Delete a user from data base
// Add a single user
class UserManager
{
	std::string filePath; //users data base file path

public:
	// default constructor
	UserManager();

	// 
	UserManager(const std::string& filePath_);

	void setFilePath(const std::string& filePath_);

	std::string getFilePath() const;

	// List all users
	std::vector<User> getAllUsers();

	std::vector<User> getAllUsers(const std::string& filePath_);

	// check users file contains login
	bool findLoginInBase(const std::string& filePath_, const std::string& accLogin);

	bool findLoginInBase(const std::string& accLogin);
	// function loads user data from file 
	bool getUserFromBase(const std::string& filePath_, User& acc, bool checkPass);

	bool getUserFromBase(User& acc, bool checkPass);

	// 
	bool addUserToBase(const std::string& filePath_, const User& user);

	bool addUserToBase(const User& user);
	// fuction edits a line of text that is stored in a  file. 
	//  copies everything from the original file and over-write 
	// it in a new file, making  edits along the way.  
	bool deleteUserFromBase(const std::string& filePath_, const std::string& login);

	bool deleteUserFromBase(const std::string& login);

};

