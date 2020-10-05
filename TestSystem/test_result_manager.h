#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "test_result.h"

// 
class TestResultManager
{
public:
	TestResultManager();
	TestResultManager(const std::string & filePath_);

	

	// find line in a file
	// where not completed test
	bool findNonCompletedResult(const std::string& filePath_, const std::string& login, const std::string& category, size_t testNum, TestResult& outResult);
	// find line in a file
	// where not completed test
	bool findNonCompletedResult(const std::string& category, const std::string& login, size_t testNum, TestResult& outResult);
	// remove line from file
	// if line contains not completed test
	bool removeNonCompletedResult(const std::string& filePath_, const std::string& login, const std::string& category, size_t testNum, TestResult& outResult);

	// remove line from file
	// if line contains not completed test
	bool removeNonCompletedResult(const std::string& login, const std::string& category, size_t testNum, TestResult& outResult);

	//  use std::ofstream to write test result to a file 
	void saveResult(const std::string& filePath, const TestResult& result);

	void saveResult(const TestResult& result);

	// find last test 

	// get user results
	// get user results by category
private:
	std::string filePath;

};

