#include "test_system.h"
#include "test_document_manager.h"
#include "test_result_manager.h"
// default constructor 

TestSystem::TestSystem()
{
}

// constructor  with parameters
// parameters:
// 1.  path to user data file
// 2. path to tests
// 3. path to  test results 

TestSystem::TestSystem(const std::string& usersPath,
	const std::string& testPath, const std::string& resultPath)
	: usersFilePath(usersPath), testsFilePath(testPath), resultsFilePath(resultPath),
	userManager(UserManager(usersPath))
{
}



// ======================================================
//                users functions
// ======================================================

void TestSystem::takeTest()
{
	std::string choice;
	std::string category;

	TestDocumentManager testDoc(testsFilePath);

	std::system("cls");  // clear console
	std::cout << "\n     -------------------\n";
	std::cout << "          Take test\n";
	std::cout << "     -------------------\n";
	// select category 
	std::cout << "\n     Select category: \n\n";

	// extract list of categories from test file 
	std::vector<std::string> listCategories = testDoc.getListCategories();

	// print list of categories
	for (size_t i = 0; i < listCategories.size(); i++)
	{
		std::cout << "     " << i + 1 << ". " << listCategories[i] << "\n";
	}

	std::cout << "     Other. Exit to user menu\n";
	std::cout << "\n     ";
	std::cin >> choice;    // select category

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int val = std::stoi(choice);
	if (val > (int)listCategories.size() || val < 1)
		return;

	// set category by selected index
	category = listCategories[val - 1];

	std::system("cls");  // clear console
	std::cout << "\n     -------------------\n";
	std::cout << "          Take test\n";
	std::cout << "     -------------------\n";

	//  show test numbers to the user

	size_t testsAmount = testDoc.getNumberTests(category);
	if (testsAmount <= 0)
	{
		std::cout << "\n     Tests for category " << category << " not found.";
		return;
	}
	std::cout << "\n     Select test number: 1";

	if (testsAmount > 1)
	{
		std::cout << "-" << testsAmount << std::endl;

	}
	std::cout << "     Other. Exit to user menu\n";
	std::cout << "\n     ";
	std::cin >> choice;   // select number of test

						  // check that the data entered is correct. 
	if (!helper::isNumber(choice))
	{
		return;
	}
	int testNum = std::stoi(choice);
	if (testNum <1 || testNum > (int)testsAmount)
		return;
	Node* categoryNode = testDoc.findCategory(category);
	Node* testNode = testDoc.findTest(categoryNode, testNum);

	// extract test: questions and answers from tests file
	if (!testNode)
	{
		std::cout << "\n     Error in extracting test " << testNum << "of category " << category << ".";
		return;
	}


	TestResultManager resultManager(resultsFilePath);


	// check that test completed here
	//   if no   
	// select 
	// 1. continue
	// 2. start from begin


	TestResult oldResult;   // object for checking  test completed 
	bool nonCompleted = resultManager.findNonCompletedResult(currentUser.getLogin(), category, testNum, oldResult);

	size_t startQuestion = 0;
	size_t answeredQuestions = 0;
	size_t rightAnswers = 0;

	// if test not completed print menu
	if (nonCompleted)
	{
		std::cout << "\n     Test " << testNum << " not completed: \n";
		std::cout << "     1. Continue test\n";
		std::cout << "     2. Start test at the beginning\n";
		std::cout << "     Other. Exit to user menu\n";
		std::cout << "\n     ";
		std::cin >> choice;   // select number of test

		if (choice == "1")
		{
			// 
			startQuestion = oldResult.getAnsweredQuestions();
			answeredQuestions = oldResult.getAnsweredQuestions();
			rightAnswers = oldResult.getRightAnswers();
		}
		else if (choice == "2")
		{
			// nothing 
		}
		else
			return;
	}

	// login, category and number of test put to TestResult object
	TestResult result;
	if (!nonCompleted)
	{
		result.setLogin(currentUser.getLogin());
		result.setCategory(category);
		result.setNumber(choice);
		result.setQuestionsNumber(testNode->children.size());
	}
	else
		result = oldResult;  // set old results to new 


	// loop through test questions  with the for loop.
	// 
	for (size_t i = startQuestion;   // start question 
		
		i < testNode->children.size(); i++)
	{
		std::string choice;
		std::system("cls");  // clear console
		std::cout << "  Question " << i + 1 << ":\n\n";
		//	std::cout << "     -------------------\n";
		// 
		std::cout /*<< "     "*/ << testNode->children[i]->text << std::endl;
		std::cout << std::endl;
		std::cout << "  Answers:\n\n";

		for (size_t j = 0; j < testNode->children[i]->children.size(); j++)
		{
			std::cout << "  " << j + 1 << ". " << testNode->children[i]->children[j]->text << std::endl;
		}

		std::cout << "\n  Q/q. Stop test" << std::endl;

		std::cout << "\n     Select: ";
		std::cin >> choice;  // select answer 

	
		if (choice == "q" || choice == "Q")
			break;

		// sum right answers 
		if (choice == std::to_string(testDoc.getRightAnswer(testNode->children[i])))
			rightAnswers++;

		// sum questions 
		answeredQuestions++;
	}

	result.setAnsweredQuestions(answeredQuestions);
	result.setRightAnswers(rightAnswers);

	// calculate persent and grade 
	result.calculate();

	// print result to console 
	viewSingleTestResult(result);

	if (nonCompleted)
	{
	    if(!	resultManager.removeNonCompletedResult(currentUser.getLogin(), category, testNum, oldResult))
			std::cout << "     \nError in deleting old test result from file.\n";
	}

	// result of test save to file 
	resultManager.saveResult(result);

}

void TestSystem::userMenu()
{
	while (true)
	{
		std::system("cls");
		std::cout << "\n     -------------------\n";
		std::cout << "          User Menu\n";
		std::cout << "     -------------------\n";
		// 

		std::cout << "\n     1. Take tests\n";
		std::cout << "     2. View tests results\n";
		std::cout << "     3. User accaunt\n";
		std::cout << "     4. Log out and exit main menu\n";
		std::cout << "\n     Select: ";

		std::string choice;
		std::cin >> choice;


		if (choice == "1")
		{
			std::cout << "Take tests\n";
			takeTest();
			std::cin.get();
			std::cin.get();
		}
		else if (choice == "2")
		{
			std::cout << "View tests results\n";
			viewTestsResults(currentUser.getLogin());
			std::cin.get();
			std::cin.get();
		}
		else if (choice == "3")
		{
			std::cout << "User data\n";
			editUserData(currentUser);
			std::cin.get();
			std::cin.get();
		}

		else if (choice == "4")
		{
			return;  // log out 
		}
	}
}

// After passing the test, user sees the test result,
// the number of correct answers,
// the percentage of correct answers and the score received.

void TestSystem::viewSingleTestResult(const TestResult& res)
{
	std::system("cls");  // clear console
	std::cout << "\n     -------------------\n";
	std::cout << "          Test result\n";
	std::cout << "     -------------------\n";
	std::cout << "\n     Category:              " << res.getCategory() << std::endl;
	std::cout << "     Test number:           " << res.getNumber() << std::endl;
	std::cout << "     Questions:             " << res.getAmountQuestions() << std::endl;
	std::cout << "     Answered questions:    " << res.getAnsweredQuestions() << std::endl;
	std::cout << "     Right answers:         " << res.getRightAnswers() << std::endl;
	std::cout << "     Right answers percent: " << res.getPercent() << std::endl;
	std::cout << "     Grade:                 " << res.getGrade() << std::endl;
}


// ======================================================
//                Admin
// ======================================================
// print  list of all users and their data

bool TestSystem::viewListUsers()
{
	std::ifstream file(usersFilePath);
	if (file.is_open()) // Check if a file is open
	{
		std::system("cls");
		std::cout << "\n ----------------+-----------------------------+----------------- " << std::endl;
		std::cout << "  Login          | Full name                   | Phone       " << std::endl;

		size_t counter = 0;
		std::string line;

		// read file line by line 
		while (std::getline(file, line))
		{
			if (!line.empty())
			{
				std::istringstream iss(line);
				User user;

				if (!(iss >> user))
				{
					std::cout << "\n     Error in reading file " << usersFilePath << std::endl;
					file.close();
					return false;
				}
				std::string full_name = user.getSurname() + " " + user.getFirstName() + " " + user.getPatronymic();

				std::cout << " ----------------+-----------------------------+----------------- " << std::endl;
				std::printf("   %-14s%-2s%-28s%-2s%s\n",
					user.getLogin().c_str(), "|", full_name.c_str(), "|", user.getPhone().c_str());
				counter++;
				if (counter > 10)
				{
					counter = 0;
					std::cout << "\n     Press Enter to continue...\n";

					std::cin.get();
					std::cin.get();
					std::system("cls");
					std::cout << "\n ----------------+-----------------------------+----------------- " << std::endl;
					std::cout << "  Login          | Full name                   | Phone       " << std::endl;
				}
			}
		}

		std::cout << " ----------------+-----------------------------+----------------- " << std::endl;
		file.close();
	}
	else
		std::cout << "\n     Error in opening file " << usersFilePath << " for reading." << std::endl;
	return false;
}

// admin operations with users

void TestSystem::workWithUsers()
{
	while (true)
	{

		std::string choice;
		std::system("cls");
		std::cout << "\n     -------------------\n";
		std::cout << "          Users data\n";
		std::cout << "     -------------------\n";
		// 
		std::cout << "\n     1. View users data\n";
		std::cout << "     2. Create user\n";
		std::cout << "     3. Delete user\n";
		std::cout << "     4. Edit user\n";
		std::cout << "     5. Exit admin menu\n";
		std::cout << "\n     Select: ";
		std::cin >> choice;


		if (choice == "1")
		{

			std::cout << "View users data\n";
			viewListUsers();
			std::cin.get();
			std::cin.get();
		}


		else if (choice == "2")
		{

			std::system("cls");
			std::cout << "\n     -------------------\n";
			std::cout << "          Create user\n";
			std::cout << "     -------------------\n\n";

			User user;
			if (createUser(user))
			{
				userManager.addUserToBase(user);
				std::cout << "\n     User was successfully created.";
			}
			else
				std::cout << "\n     Create user failed.\n";
			std::cin.get();
			std::cin.get();

		}
		else if (choice == "3")
		{
			std::system("cls");
			std::cout << "\n     -------------------\n";
			std::cout << "          Delete user\n";
			std::cout << "     -------------------\n\n";


			if (deleteUser())
			{
				std::cout << "\n     User was successfully deleted.";
			}
			else
				std::cout << "\n     Delete user failed.\n";

			std::cin.get();
			std::cin.get();
		}
		else if (choice == "4")
		{
			User acc;
			std::system("cls");
			std::cout << "\n     -------------------\n";
			std::cout << "          Edit user\n";
			std::cout << "     -------------------\n\n";
			std::cout << "     Enter login: \n     ";

			std::string tmpStr;
			std::cin >> tmpStr;
			acc.setLogin(tmpStr);

			if (userManager.getUserFromBase(acc, false))
			{
				// any user data can be changed
				// only the admin has this option.
				editUserData(acc);
			}
			else
				std::cout << "\n     User with login " << tmpStr << " not found.";
			std::cin.get();
			std::cin.get();
		}
		else if (choice == "5")
		{
			// return to admin menu  
			return;
		}
	}
}

bool TestSystem::addCategory()
{
	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Add category section\n";
	std::cout << "     ---------------------------\n\n";
	try
	{
		TestDocumentManager  testsManager(testsFilePath);

		if (!testsManager.documentIsEmpty())
		{
			std::vector<std::string> listCategories = testsManager.getListCategories();

			std::cout << "\n     Existing categories:\n\n";
			for (auto& c : listCategories)
				std::cout << "       " << c << " \n";
		}
		else std::cout << "\n     Empty document. No categories yet.\n";

		std::cout << "\n     Enter new category or";
		std::cout << "\n     Q/q exit to menu" << std::endl;
		std::cout << "     ";
		std::string newCategory;
		std::cin >> newCategory;
		if (newCategory == "q" || newCategory == "Q")
			return false;
		if (!testsManager.documentIsEmpty() && testsManager.findCategory(newCategory))
			// already exist
		{
			std::cout << "\n     This category already exist.";
			return false;
		}

		testsManager.addCategory(newCategory);
		testsManager.saveTree();
		std::cout << "\n     Category added successfully.";
		return true;
	}
	catch (const std::exception& ex)
	{
		std::cout << "     " << ex.what() << std::endl;
		return false;
	}
}

void TestSystem::addTest()
{
	std::system("cls");
	std::cout << "\n     -----------------------\n";
	std::cout << "          Add test section\n";
	std::cout << "     -----------------------\n";

	TestDocumentManager  testsManager(testsFilePath);

	if (testsManager.documentIsEmpty())
	{
		std::cout << "\n     Empty document. No categories yet.\n";
		std::cout << "\n     Category must first be established.\n";
		return;
	}



	std::vector<std::string> listCategories = testsManager.getListCategories();
	size_t n = 1;
	//	std::cout << "\n     Existing categories:\n\n";
	std::cout << "\n     Select category where test will be added:\n\n";
	for (auto& c : listCategories)
	{
		std::cout << "       " << n << ". " << c << " \n";
		n++;
	}


	std::cout << "       Other. Exit to user menu\n";
	std::cout << "\n     ";
	std::string choice;

	std::cin >> choice;    // select category

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int val = std::stoi(choice);
	if (val > (int)listCategories.size() || val < 1)
		return;

	// set category by selected index
	std::string category = listCategories[val - 1];
	auto categoryNode = testsManager.findCategory(category);
	if (!categoryNode)
	{
		std::cout << "\n     Error. Category not found";
		return;
	}

	std::system("cls");
	std::cout << "\n     -----------------------\n";
	std::cout << "          Add test section\n";
	std::cout << "     -----------------------\n\n";

	if (categoryNode->children.empty())
	{
		std::cout << "\n     No test for this category yet.\n";
	}
	else
	{
		std::cout << "\n     Existing tests:\n\n";
		for (size_t i = 0; i < categoryNode->children.size(); i++)
		{
			std::cout << "       " << i + 1 << " \n";
		}
	}

	std::cout << "\n       Press Y/y to add new test section \n";
	std::cout << "       Other. Exit to menu\n";

	std::cout << "\n     ";

	std::cin >> choice;    // select 
	if (choice != "y" && choice != "Y")
	{
		return;
	}
	size_t testNumber = categoryNode->children.size();
	testNumber += 1;
	if (
		testsManager.addTest(category, testNumber))
	{
		testsManager.saveTree();

		std::cout << "\n     Test section added successfully.";
	}
}

void TestSystem::addQuestion()
{
	std::system("cls");


	TestDocumentManager  testsManager(testsFilePath);

	if (testsManager.documentIsEmpty())
	{
		std::cout << "\n     Empty document. No categories yet.\n";
		std::cout << "\n     Category must first be established.\n";
		return;
	}

	std::cout << "\n     ---------------------------\n";
	std::cout << "          Add question section\n";
	std::cout << "     ---------------------------\n";

	std::vector<std::string> listCategories = testsManager.getListCategories();
	size_t n = 1;
	//	std::cout << "\n     Existing categories:\n\n";
	std::cout << "\n     Select category where question will be added:\n\n";
	for (auto& c : listCategories)
	{
		std::cout << "       " << n << ". " << c << " \n";
		n++;
	}


	std::cout << "       Other. Exit to menu\n";
	std::cout << "\n     ";
	std::string choice;

	std::cin >> choice;    // select category

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int val = std::stoi(choice);
	if (val > (int)listCategories.size() || val < 1)
		return;

	// set category by selected index
	std::string category = listCategories[val - 1];
	auto categoryNode = testsManager.findCategory(category);
	if (!categoryNode)
	{
		std::cout << "\n     Error. Category not found";
		return;
	}

	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Add question section\n";
	std::cout << "     ---------------------------\n\n";

	size_t testNumber = 1;
	if (categoryNode->children.empty())
	{
		std::cout << "\n     No test for this category yet.\n";
		std::cout << "\n     Test section must first be established.\n";
		return;
	}
	else
	{

		std::cout << "\n     Select test where question will be added:\n\n";
		for (size_t i = 0; i < categoryNode->children.size(); i++)
		{
			std::cout << "       " << i + 1 << " \n";
		}
		std::cout << "       Other. Exit to  menu\n";

		std::cout << "\n     ";
		std::string choice;

		std::cin >> choice;    // select test number

							   // check that the data entered is correct. 
		if (!helper::isNumber(choice))
			return;

		int val = std::stoi(choice);
		if (val > (int)categoryNode->children.size() || val < 1)
			return;

		testNumber = val;
	}
	std::cin.ignore();

	std::system("cls");
	/*	std::cout << "\n     -------------------\n";
	std::cout << "          Add test\n";
	std::cout << "     -------------------\n\n";
	std::cout << "       Category:    " << category << " \n";
	std::cout << "       Test number: " << testNumber << " \n";*/
	std::cout << "\n     Enter question text for test " << testNumber;
	std::cout << "\n     Q/q. Exit to menu" << std::endl;
	std::cout << "\n     ";
	std::string newQuestionText;
	//std::cin.ignore();
	std::getline(std::cin, newQuestionText);
	//std::cin >> std::noskipws >> newQuestionText;
	//std::cin.setf(std::ios::skipws);
	//std::cin >> newQuestionText;
	//std::cin.unsetf(std::ios::skipws);
	if (newQuestionText == "q" || newQuestionText == "Q")
		return;
	std::cin.ignore();
	size_t questionNumber = categoryNode->children[testNumber - 1]->children.size();
	/*if (questionsAmount == 0)*/
	questionNumber += 1;


	testsManager.addQuestion(category, testNumber, questionNumber, newQuestionText);
	testsManager.saveTree();




	//if (editor.insert_item_test(category))
	std::cout << "\n     Question added successfully.";;
}

void TestSystem::addAnswer()
{
	std::system("cls");

	std::cout << "\n     ---------------------------\n";
	std::cout << "          Add answer\n";
	std::cout << "     ---------------------------\n";
	TestDocumentManager  testsManager(testsFilePath);

	if (testsManager.documentIsEmpty())
	{
		std::cout << "\n     Empty document. No categories yet.\n";
		std::cout << "\n     Category must first be established.\n";
		return;
	}


	std::vector<std::string> listCategories = testsManager.getListCategories();
	size_t n = 1;
	//	std::cout << "\n     Existing categories:\n\n";
	std::cout << "\n     Select category where answer will be added:\n\n";
	for (auto& c : listCategories)
	{
		std::cout << "       " << n << ". " << c << " \n";
		n++;
	}


	std::cout << "       Other. Exit to menu\n";
	std::cout << "\n     ";
	std::string choice;

	std::cin >> choice;    // select category

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int val = std::stoi(choice);
	if (val > (int)listCategories.size() || val < 1)
		return;

	// set category by selected index
	std::string category = listCategories[val - 1];
	auto categoryNode = testsManager.findCategory(category);
	if (!categoryNode)
	{
		std::cout << "\n     Error. Category not found";
		return;
	}

	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Add answer\n";
	std::cout << "     ---------------------------\n\n";


	if (categoryNode->children.empty())
	{
		std::cout << "\n     No test for this category yet.\n";
		std::cout << "\n     Test section must first be established.\n";
		return;
	}


	std::cout << "\n     Select test number where answer will be added:\n\n";
	for (size_t i = 0; i < categoryNode->children.size(); i++)
	{
		std::cout << "       " << i + 1 << " \n";
	}
	std::cout << "       Other. Exit to menu\n";

	std::cout << "\n     ";
	//std::string choice;

	std::cin >> choice;    // select test number

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int testNumber = std::stoi(choice);
	if (testNumber > (int)categoryNode->children.size() || testNumber < 1)
		return;

	//testNumber -= 1;

	std::cin.ignore();

	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Add answer\n";
	std::cout << "     ---------------------------\n\n";
	std::cout << "\n     Select question number where answer will be added:\n\n";
	for (size_t i = 0; i < categoryNode->children[testNumber - 1]->children.size(); i++)
	{
		std::cout << "       " << i + 1 << " \n";
	}
	std::cout << "       Other. Exit to menu\n";

	std::cout << "\n     ";
	std::cin >> choice;    // select question  number

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int questionNumber = std::stoi(choice);
	if (questionNumber > (int)categoryNode->children[testNumber - 1]->children.size() || questionNumber < 1)
		return;

	std::cin.ignore();

	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Add answer\n";
	std::cout << "     ---------------------------\n\n";

	std::cout << "\n     Enter answer text";
	std::cout << "\n     Q/q. Exit to menu" << std::endl;
	std::cout << "\n     ";
	std::string answerText;

	std::getline(std::cin, answerText);

	if (answerText == "q" || answerText == "Q")
		return;
	std::cin.ignore();
	size_t ansNumber = categoryNode->children[testNumber - 1]->children[questionNumber - 1]->children.size();

	ansNumber += 1;


	testsManager.addAnswer(category, testNumber, questionNumber, ansNumber, answerText);
	testsManager.saveTree();

	std::cout << "\n     Answer added successfully.";
}

void TestSystem::setRightAnswer()
{
	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Set right answer\n";
	std::cout << "     ---------------------------\n";
	TestDocumentManager  testsManager(testsFilePath);

	if (testsManager.documentIsEmpty())
	{
		std::cout << "\n     Empty document. No categories yet.\n";
		std::cout << "\n     Category must first be established.\n";
		return;
	}

	

	std::vector<std::string> listCategories = testsManager.getListCategories();
	size_t n = 1;
	//	std::cout << "\n     Existing categories:\n\n";
	std::cout << "\n     Select category to set right answer\n\n";
	for (auto& c : listCategories)
	{
		std::cout << "       " << n << ". " << c << " \n";
		n++;
	}


	std::cout << "       Other. Exit to menu\n";
	std::cout << "\n     ";
	std::string choice;

	std::cin >> choice;    // select category

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int val = std::stoi(choice);
	if (val > (int)listCategories.size() || val < 1)
		return;

	// set category by selected index
	std::string category = listCategories[val - 1];
	auto categoryNode = testsManager.findCategory(category);
	if (!categoryNode)
	{
		std::cout << "\n     Error. Category not found";
		return;
	}

	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Set right answer\n";
	std::cout << "     ---------------------------\n";


	if (categoryNode->children.empty())
	{
		std::cout << "\n     No test for this category yet.\n";
		std::cout << "\n     Test section must first be established.\n";
		return;
	}


	std::cout << "\n     Select test number to set right answer:\n\n";
	for (size_t i = 0; i < categoryNode->children.size(); i++)
	{
		std::cout << "       " << i + 1 << " \n";
	}
	std::cout << "       Other. Exit to menu\n";

	std::cout << "\n     ";
	//std::string choice;

	std::cin >> choice;    // select test number

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int testNumber = std::stoi(choice);
	if (testNumber > (int)categoryNode->children.size() || testNumber < 1)
		return;


	std::cin.ignore();

	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Set right answer\n";
	std::cout << "     ---------------------------\n";
	std::cout << "\n     Select question number to set right answer:\n\n";
	for (size_t i = 0; i < categoryNode->children[testNumber - 1]->children.size(); i++)
	{
		std::cout << "       " << i + 1 << " \n";
	}
	std::cout << "       Other. Exit to menu\n";

	std::cout << "\n     ";
	std::cin >> choice;    // select question  number

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int questionNumber = std::stoi(choice);
	if (questionNumber > (int)categoryNode->children[testNumber - 1]->children.size() || questionNumber < 1)
		return;

	size_t answers = categoryNode->children[testNumber - 1]->children[questionNumber - 1]->children.size();
	if (answers == 0)
	{
		std::cout << "\n     No answers yet.\n";
		std::cout << "\n     Answers must first be established.\n";
		return;
	}
	std::system("cls");
	std::cout << "\n     ---------------------------\n";
	std::cout << "          Set right answer\n";
	std::cout << "     ---------------------------\n";
	std::cout << "\n     Select answer number:\n\n";
	for (size_t i = 0; i < answers; i++)
	{
		std::cout << "       " << i + 1 << " \n";
	}
	std::cout << "       Other. Exit to menu\n";

	std::cout << "\n     ";
	std::cin >> choice;    // select answer  number

						   // check that the data entered is correct. 
	if (!helper::isNumber(choice))
		return;

	int answNumber = std::stoi(choice);
	if (answNumber > (int)answers || answNumber < 1)
		return;

	if (testsManager.setRightAnswer(categoryNode->children[testNumber - 1]->children[questionNumber - 1], answNumber))
	{
		testsManager.saveTree();

		std::cout << "\n     Answer set successfully.";
	}
}



void TestSystem::workWithTests()
{
	while (true)
	{

		std::string choice;
		std::system("cls");
		std::cout << "\n     ------------------------\n";
		std::cout << "          Edit test files\n";
		std::cout << "     ------------------------\n";
		// 
		std::cout << "\n     1. Add category section\n";
		std::cout << "     2. Add test  section\n";
		std::cout << "     3. Add question  section\n";
		std::cout << "     4. Add answers\n";
		std::cout << "     5. Set right answer\n";
		std::cout << "     6. Exit admin menu\n";
		std::cout << "\n     Select: ";
		std::cin >> choice;


		if (choice == "1")
		{
			addCategory();
			std::cin.get();
			std::cin.get();
		}


		else if (choice == "2")
		{
			addTest();
			std::cin.get();
			std::cin.get();

		}
		else if (choice == "3")
		{
			addQuestion();
			std::cin.get();
			std::cin.get();
		}
		else if (choice == "4")
		{
			addAnswer();
			std::cin.get();
			std::cin.get();
		}
		else if (choice == "5")
		{
			setRightAnswer();
			std::cin.get();
			std::cin.get();
		}
		else if (choice == "6")
		{
			// return to admin menu 
			return;
		}
	}
}

void TestSystem::adminMenu()
{
	while (true)
	{
		std::system("cls");  // clear console
		std::cout << "\n     -------------------\n";
		std::cout << "          Admin Menu\n";
		std::cout << "     -------------------\n";
		std::cout << "\n     1. Work with users\n";
		std::cout << "     2. Edit test files\n";
		std::cout << "     3. Wiew tests results\n";
		std::cout << "     4. Admin accaunt\n";
		std::cout << "     5. Log out and exit main menu\n";
		std::cout << "\n     Select: ";

		std::string choice;
		std::cin >> choice;

		if (choice == "1")
		{
			// - user management – creating, deleting, modifying users
			workWithUsers();

			std::cin.get();
			std::cin.get();
		}


		else if (choice == "2")
		{
			workWithTests();
			std::cin.get();
			std::cin.get();

		}
		else if (choice == "3")
		{
			std::cout << "view tests results\n";
			std::system("cls");
			std::cout << "\n     -------------------\n";
			std::cout << "          Test results\n";
			std::cout << "     -------------------\n";
			std::cout << "\n     Enter user login you want get results: \n\n     ";

			std::string login;
			std::cin >> login;


			// check loginin base

			if (!userManager.findLoginInBase(login))
			{
				std::cout << "\n     This user name not found.\n";

			}
			else
				viewTestsResults(login);
			std::cin.get();
			std::cin.get();
		}
		else if (choice == "4")
		{
			editUserData(currentUser);
			std::cin.get();
			std::cin.get();
		}

		else if (choice == "5")
		{
			return; // log out 
		}
	}
}

//------------------------------------
//  common functions 
//---------------------------------------

// When a user needs to take a test , they must first log in.
// Users log into system  by entering their User name and Password
bool TestSystem::logIn()
{
	std::system("cls");  // clear console 
	std::cout << "\n     ---------------------\n";
	std::cout << "            Log in\n";
	std::cout << "     ---------------------\n\n";

	// reset curent accaunt, cleaning each member.
	currentUser = User();

	// input login, password
	std::string tmpStr;
	std::cout << "     Enter login: \n     ";
	std::cin >> tmpStr;
	currentUser.setLogin(tmpStr);

	std::cout << "     Enter password: \n     ";
	std::cin >> tmpStr;
	currentUser.setPassword(chipher::encrypt(tmpStr));

	// check login, password in base and load user
	if (userManager.getUserFromBase(currentUser, true))
	{
		// set user role
		if (currentUser.getLogin() == "admin")
			currentUser.setRole(Role::Admin);

		return true;  // success
	}

	return false;  // if failure 
}

// New users must register with the system before taking tests. 
// This includes choosing a username and password 
//and  users enter their information : full name and phone

bool TestSystem::registration()
{
	std::system("cls");  // clear console
	std::cout << "\n     ---------------------\n";
	std::cout << "          Registration\n";
	std::cout << "     ---------------------\n\n";

	// input login, password, full name and phone
	User user;
	if (!createUser(user))
		return false;

	// 
	userManager.addUserToBase(user);

	return true;
}

bool TestSystem::createUser(User& user)
{
	std::string tmpStr;
	std::cout << "     Enter login: \n     ";
	std::cin >> tmpStr;

	// check loginin base
	if (userManager.findLoginInBase(tmpStr))
	{
		std::cout << "\n     This user name already exists.\n";
		return false;
	}

	user.setLogin(tmpStr);
	// input password, full name, phone

	std::cout << "     Enter password: \n     ";
	std::cin >> tmpStr;
	user.setPassword(chipher::encrypt(tmpStr));

	std::cout << "     Enter surname: \n     ";
	std::cin >> tmpStr;
	user.setSurname(tmpStr);
	std::cout << "     Enter first name: \n     ";
	std::cin >> tmpStr;
	user.setFirstName(tmpStr);
	std::cout << "     Enter patronymic: \n     ";
	std::cin >> tmpStr;
	user.setPatronymic(tmpStr);

	std::cout << "     Enter phone: \n     ";
	std::cin >> tmpStr;
	user.setPhone(tmpStr);

	return true;
}

bool TestSystem::deleteUser()
{
	std::cout << "     Enter login: \n     ";
	std::string login;
	std::cin >> login;

	// check login in users file 

	if (!userManager.findLoginInBase(login))
	{
		std::cout << "\n     The username was not found in system.\n";
		return false;
	}

	userManager.deleteUserFromBase(login);

	return true;
}
// main program menu 

void TestSystem::mainMenu()
{
	while (true)
	{
		// reset curent accaunt, cleaning each member.
		currentUser = User();

		std::string choice;
		std::system("cls");  // clear console
		std::cout << "\n     ---------------\n";
		std::cout << "          Menu\n";
		std::cout << "     ---------------\n";
		std::cout << "\n     1. Log in\n";
		std::cout << "     2. Registration\n";

		std::cout << "     3. Exit\n";
		std::cout << "\n     Select: ";
		std::cin >> choice;

		if (choice == "1")
		{
			// After successfully logging in,
			// admin or user menu will appear.
			if (logIn())
			{
				if (currentUser.isAdmin())
				{
					adminMenu();
				}
				else
				{
					userMenu();
				}
			}
			else
				std::cout << "\n     Login failed.\n";

			std::cin.get();
			std::cin.get();

		}
		else if (choice == "2")
		{
			// after registration 
			// user returns to main menu 
			if (registration())
			{
				std::cout << "\n     Registration successful!\n";
			}
			else
				std::cout << "\n     Registration failed.\n";

			std::cin.get();
			std::cin.get();
		}
		else if (choice == "3")  // exit program
		{
			std::cout << "     Bye!\n";
			return;
		}
	}
}

// view results of an individual user 
// parametr: user login

void TestSystem::viewTestsResults(const std::string& login)
{
	std::system("cls");  // clear console
	std::cout << "\n   ---------------------------------------------------------\n";
	std::cout << "       Results user '" << login << "'" << std::endl;
	std::cout << "   ---------------------------------------------------------\n";

	std::ifstream file(resultsFilePath);
	if (file.is_open()) // Check if a file is open
	{
		std::string line;
		size_t counter = 0;
		bool flag = false;

		// read file line by line 
		while (std::getline(file, line))
		{
			if (!line.empty())
			{
				// create stream object from line
				std::istringstream iss(line);
				TestResult tmp_result;

				// fill result object
				if (!(iss >> tmp_result))  // check error reading
				{
					std::cout << "\n     Error in reading file " << resultsFilePath << std::endl;
					file.close();
					return;
				}

				// if login is found  print test results 
				if (tmp_result.getLogin() == login)
				{
					flag = true;
					counter++;
					if (counter > 5)
					{
						counter = 0;
						std::cout << "\n     Press Enter to continue...\n";

						std::cin.get();
						std::cin.get();
						std::system("cls");  // clear console
						std::cout << "\n   ---------------------------------------------------------\n";
						std::cout << "       Results user '" << login << "'" << std::endl;
						std::cout << "   ---------------------------------------------------------\n";
					}
					printf("     Category: %s Test No: %s Questions: %d ", tmp_result.getCategory().c_str(), tmp_result.getNumber().c_str(), tmp_result.getAmountQuestions());
					printf("\n     Completed questions: %d Right answers: %d", tmp_result.getAnsweredQuestions(), tmp_result.getRightAnswers());
					printf("\n     Grade: %d Right answers %s: %d", tmp_result.getGrade(), "%", tmp_result.getPercent());

					std::cout << "\n   ---------------------------------------------------------\n";
				}
			}
		}

		if (!flag)  // if login not found  
			std::cout << "\n     There are no results." << std::endl;
		file.close();
	}
	else
		std::cout << "\n     Error in opening file " << resultsFilePath << " for reading." << std::endl;
}

void TestSystem::editUserData(User& acc)
{
	bool edited = false;
	while (true)
	{
		viewUserData(acc);
		std::string tmpStr;
		std::string choice;

		std::cout << "\n     -------------------\n";
		std::cout << "          Edit data\n";
		std::cout << "     -------------------\n";
		// 
		std::cout << "\n     1. Change password\n";
		std::cout << "     2. Change surname\n";
		std::cout << "     3. Change first name\n";
		std::cout << "     4. Change patronymic\n";
		std::cout << "     5. Change phone\n";
		std::cout << "     6. Exit menu\n";
		std::cout << "\n     Select: ";
		std::cin >> choice;

		if (choice == "1")
		{
			std::cout << "     Enter new password: \n     ";
			std::cin >> tmpStr;
			acc.setPassword(chipher::encrypt(tmpStr));
			edited = true;
		}
		else if (choice == "2")
		{
			std::cout << "     Enter new surname: \n     ";
			std::cin >> tmpStr;
			acc.setSurname(tmpStr);
			edited = true;
		}
		else if (choice == "3")
		{
			std::cout << "     Enter new first name: \n     ";
			std::cin >> tmpStr;
			acc.setFirstName(tmpStr);
			edited = true;
		}
		else if (choice == "4")
		{
			std::cout << "     Enter new patronymic: \n     ";
			std::cin >> tmpStr;
			acc.setPatronymic(tmpStr);
			edited = true;
		}
		else if (choice == "5")
		{
			std::cout << "     Enter new phone: \n     ";
			std::cin >> tmpStr;
			acc.setPhone(tmpStr);
			edited = true;
		}
		else if (choice == "6")
		{
			break;
		}
	}

	if (edited)
	{
		// rewrite user data 
		userManager.deleteUserFromBase(acc.getLogin());
		userManager.addUserToBase(acc);
	}
}

void TestSystem::viewUserData(const User& acc)
{
	std::system("cls");

	std::cout << "\n     -------------------\n";
	std::cout << "          User data\n";
	std::cout << "     -------------------\n";
	// 

	std::cout << "\n     Login:     " << acc.getLogin() << std::endl;
	std::cout << "     Password:  " << chipher::decrypt(acc.getPassword()) << std::endl;
	std::cout << "     Full name: " << acc.getSurname() << " " << acc.getFirstName() << " " << acc.getPatronymic() << " " << std::endl;
	std::cout << "     Phone:     " << acc.getPhone() << std::endl;

}
