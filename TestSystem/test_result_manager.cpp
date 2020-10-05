#include "test_result_manager.h"



TestResultManager::TestResultManager()
{
}

// constructor with parameters
TestResultManager::TestResultManager(const std::string& filePath_): filePath(filePath_)
{
}

// find line in a file
// where not completed test

inline bool TestResultManager::findNonCompletedResult(const std::string& filePath_, const std::string& login, const std::string& category, size_t testNum, TestResult& outResult)
{
// check that test completed here
// read lines 
// result: user , category, test number
// if questions ! answers
// read this line to result  
	std::ifstream file(filePath_);  // open file for reading

	if (file.is_open()) // Check if a file is open
	{
		TestResult testRes;

		// read file line by line 
		// &  get first word of line
		while (file >> testRes)
		{
			if (testRes.getLogin() == login && testRes.getCategory() == category && testRes.getNumber() == std::to_string(testNum)
				&& testRes.getAmountQuestions() != testRes.getAnsweredQuestions())
			{
				outResult = testRes;  
				file.close();
				return true;
			}
		}
		file.close();
	}
	else
		std::cout << "\n     Error in opening file: " << filePath_ << std::endl;
	return false;
}

// find line in a file
	// where not completed test
bool TestResultManager::findNonCompletedResult(const std::string& login, const std::string& category, size_t testNum, TestResult& outResult)
{
	return findNonCompletedResult(filePath, login, category,  testNum, outResult);
}


// when  test is passed delete line 
 bool TestResultManager::removeNonCompletedResult(const std::string& filePath_, const std::string& login, const std::string& category, size_t testNum, TestResult& outResult)
{
	// change name of file 
	std::string oldFilePath = filePath_ + "~";
	std::rename(filePath_.c_str(), oldFilePath.c_str());

	// open file for reading
	std::ifstream oldFile(oldFilePath);
	if (!oldFile.is_open()) // Check if a file is open
	{
		std::cout << "\n     Error in opening file for reading\n";
		return false;
	}
	// open new file for writing
	std::ofstream newFile(filePath_);
	if (!newFile.is_open()) // Check if a file is open
	{
		std::cout << "\n     Error in opening file for writing\n";
		return false;
	}

	std::string line;

	// read file line by line 
	while (std::getline(oldFile, line))
	{
		TestResult tmpRes;
		//  convert line to result object
		std::istringstream iss(line);
		if (!(iss >> tmpRes))
		{
			std::cout << "\n     Error in reading file " << filePath_ << std::endl;
			oldFile.close();
			newFile.close();
			return false;
		}
		if (!(tmpRes.getLogin() == login && tmpRes.getCategory() == category && tmpRes.getNumber() == std::to_string(testNum)
			&& tmpRes.getAmountQuestions() != tmpRes.getAnsweredQuestions()))
		{
			newFile << line << '\n';
		}
	}

	oldFile.close();
	newFile.close();

	// old file should be removed
	std::remove(oldFilePath.c_str());
	return true;
}

bool TestResultManager::removeNonCompletedResult(const std::string& login, const std::string& category, size_t testNum, TestResult& outResult)
{
	return removeNonCompletedResult(filePath, login, category, testNum, outResult);;
}
// when test is passed
//  use std::ofstream to write test result to a file 
void TestResultManager::saveResult(const std::string& filePath_, const TestResult& result)
{
	// flag std::ios::app -- output operations happen at the end of the file,
	// appending to its existing contents.
	std::ofstream file(filePath_, std::ios::out | std::ios::app);

	// if file is not open then get out of the function
	if (!file.is_open())
	{
		std::cout << "\n     Error in opening file for writing: "
			<< filePath_ << " for writing." << std::endl;
		return;
	}

	// append test result at the end of the file
	file << result << std::endl;
	file.close();

}

void TestResultManager::saveResult( const TestResult& result)
{
	saveResult(filePath, result);
}
