#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "user.h"
#include "user_manager.h"
#include "chipher.h"
#include "test_result.h"
#include "test_parser.h"
#include "helper.h"


//  testing system 
class TestSystem
{
private:

	User currentUser;  // The current user is whoever is logged in and performing an action.
	std::string usersFilePath;
	std::string testsFilePath;
	std::string resultsFilePath;
	UserManager userManager;
public:

	// default constructor 
	TestSystem();

	// constructor  with parameters
	// parameters:
	// 1.  path to user data file
	// 2. path to tests
	// 3. path to  test results 
	TestSystem(const std::string& usersPath, const std::string& testPath, const std::string& resultPath);

	


	// ======================================================
	//                users functions
	// ======================================================
	void takeTest();

	void userMenu();


	// ======================================================
	//                Admin
	// ======================================================

	// print  list of all users and their data
	bool viewListUsers();

	// admin operations with users
	void workWithUsers();

	bool addCategory();

	void addTest();

	void addQuestion();

	void addAnswer();

	void setRightAnswer();

	void workWithTests();
	//

	void adminMenu();

	//------------------------------------
	//  common functions 
	//---------------------------------------

	// When a user needs to take a test , they must first log in.
	// Users log into system  by entering their User name and Password
	bool logIn();

	// New users must register with the system before taking tests. 
	// This includes choosing a username and password 
	//and  users enter their information : full name and phone
	bool registration();

	bool createUser(User& user);

	bool deleteUser();

	// After passing the test, user sees the test result,
	// the number of correct answers,
	// the percentage of correct answers and the score received.
	void viewSingleTestResult(const TestResult& res);

	// view results of an individual user 
	// parametr: user login
	void viewTestsResults(const std::string& login);

	void editUserData(User& acc);

	void viewUserData(const User& acc);
	// main program menu 
	void mainMenu();
};

