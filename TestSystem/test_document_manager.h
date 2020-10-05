#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "test_parser.h"
class TestDocumentManager
{
	Node* root;  // root of document
	std::string filePath;
public:
	// default constructor
	TestDocumentManager();
	// constructor with parameters
	TestDocumentManager(const std::string& filePath);
	// destructor 
	~TestDocumentManager();

	// that an instance of a  class should not be copied at all
	// using deleted copy constructorand copy assignment operator
	TestDocumentManager(const TestDocumentManager& temp_obj) = delete;
	TestDocumentManager& operator=(const TestDocumentManager& temp_obj) = delete;


	void saveTree();
	void saveTree(Node* node, std::ofstream& os);

	// remove every node recursively
	void clear(Node* node);

	void clear();

	bool documentIsEmpty() const;

	// list of categories 
	std::vector<std::string> getListCategories();

	// find and return category node 
	Node* findCategory(const std::string& category);

	// get the amount of tests in category
	size_t getNumberTests(const std::string& category);

	// 
	void addCategory(Node* node);


	void addCategory(const std::string& newCategory);


	bool addTest(const std::string& category, Node* testNode);
	bool addTest(const std::string& category, size_t  testNum);

	bool addQuestion(const std::string& category, size_t testNum, Node* node_question);
	bool addQuestion(const std::string& category, size_t testNum, size_t questNum, const std::string& questionText);

	bool addAnswer(const std::string& category, size_t testNum,
		size_t questionNum, Node* answer_node);

	bool addAnswer(const std::string& category, size_t testNum,
		size_t questionNum, size_t answerNum, const std::string& answerText);

	Node* findTest(Node* parent, size_t num);

	Node* findQuestion(Node* parent, size_t num);

	Node* findAnswer(Node* parent, size_t ansNum);
	// set right answer for question
	// parametrs: question node , number of right answer
	bool setRightAnswer(Node* questionNode, size_t ansNum);

	// get right answer by question
	// parametr: question node 
	size_t getRightAnswer(Node* questionNode);
};
