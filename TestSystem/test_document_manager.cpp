#include "test_document_manager.h"




// default constructor

TestDocumentManager::TestDocumentManager() : root(nullptr)
{}
// constructor with parameters
TestDocumentManager::TestDocumentManager(const std::string& filePath_) : filePath(filePath_)
{

	// parse test file & get root element 
	root = TestParser::parse(filePath);
}

// destructor 

TestDocumentManager::~TestDocumentManager()
{
	// remove every node
	clear();
}

void TestDocumentManager::saveTree()
{
	//std::string file_path = "tests5.txt";
	std::ofstream file(filePath);

	if (!file.is_open())
	{
		std::cout << "\n     Error in opening file: " << filePath << std::endl;
		return;
	}

	saveTree(root, file);

	file.close();
}

void TestDocumentManager::saveTree(Node* node, std::ofstream& os)
{
	if (node->content != "Root")
	{
		if (!node->content.empty())
			os << node->content << std::endl;
		if (!node->text.empty())
			os << node->text << std::endl;
	}
	for (auto& child : node->children)
		saveTree(child, os);

}

// remove every node recursively

void TestDocumentManager::clear(Node* node)
{
	if (!node)
		return;
	for (auto& child : node->children)
		clear(child);

	delete node;
	node = nullptr;
}

void TestDocumentManager::clear()
{
	// clear start by root
	clear(root);
}

bool TestDocumentManager::documentIsEmpty() const
{
	return root == nullptr;
}

// list of categories 

std::vector<std::string> TestDocumentManager::getListCategories()
{
	std::vector<std::string> categories;
	if (!root)
		return categories;

	for (auto& categoryNode : root->children)
	{
		std::istringstream iss(categoryNode->content);
		std::string tmp, categoryName;

		iss >> tmp >> categoryName;
		categories.push_back(categoryName);
	}

	return categories;
}

// find and return category node 

Node* TestDocumentManager::findCategory(const std::string& category)
{
	if (root)
	{
		for (auto& node : root->children)
			if (node->content.find(category) != std::string::npos)
				return node;
	}
	return nullptr;
}

// get the amount of tests in category

size_t TestDocumentManager::getNumberTests(const std::string& category)
{
	Node* catNode = findCategory(category);
	if (catNode == nullptr)
		return 0;

	return catNode->children.size();
}

// 

void TestDocumentManager::addCategory(Node* node)
{
	if (!node)
		return;
	if (!root)
		root = new Node(LexemType::Root, "Root");
	root->children.push_back(node);
}

void TestDocumentManager::addCategory(const std::string& newCategory)
{
	auto node = new Node(LexemType::Category, "#category " + newCategory);
	addCategory(node);
}

bool TestDocumentManager::addTest(const std::string& category, Node* testNode)
{
	Node* categoryNode = findCategory(category);
	if (!categoryNode)
		return false;
	categoryNode->children.push_back(testNode);

	return true;
}

bool TestDocumentManager::addTest(const std::string& category, size_t testNum)
{
	Node* testNode = new Node(LexemType::Test,
		"#test " + std::to_string(testNum));
	return addTest(category, testNode);
}

bool TestDocumentManager::addQuestion(const std::string& category, size_t testNum, Node* node_question)
{
	Node* categoryNode = findCategory(category);
	if (!categoryNode)
		return false;

	Node* testNode = findTest(categoryNode, testNum);
	if (!testNode)
		return false;

	testNode->children.push_back(node_question);

	return true;
}

bool TestDocumentManager::addQuestion(const std::string& category, size_t testNum, size_t questNum, const std::string& questionText)
{

	Node* questionNode = new Node(LexemType::Question,
		"#question " + std::to_string(questNum), questionText);


	return addQuestion(category, testNum, questionNode);

}

bool TestDocumentManager::addAnswer(const std::string& category, size_t testNum, size_t questionNum, Node* answer_node)
{
	Node* categoryNode = findCategory(category);
	if (!categoryNode)
		return false;

	Node* testNode = findTest(categoryNode, testNum);
	if (!testNode)
		return false;
	Node* questionNode = findQuestion(testNode, questionNum);
	questionNode->children.push_back(answer_node);

	return true;
}

bool TestDocumentManager::addAnswer(const std::string& category, size_t testNum, size_t questionNum, size_t answerNum, const std::string& answerText)
{

	Node* answerNode = new Node(LexemType::Answer,
		"#answer " + std::to_string(answerNum), answerText);
	return addAnswer(category, testNum, questionNum, answerNode);
}

Node* TestDocumentManager::findTest(Node* parent, size_t num)
{
	num--;
	if (num >= 0 && num < parent->children.size())
		return parent->children[num];
	return nullptr;
}

Node* TestDocumentManager::findQuestion(Node* parent, size_t num)
{
	num--;
	if (num >= 0 && num < parent->children.size())
		return parent->children[num];
	return nullptr;
}

Node* TestDocumentManager::findAnswer(Node* parent, size_t ansNum)
{
	ansNum--;
	if (ansNum >= 0 && ansNum < parent->children.size())
		return parent->children[ansNum];
	return nullptr;

}

// set right answer for question
// parametrs: question node , number of right answer

bool TestDocumentManager::setRightAnswer(Node* questionNode, size_t ansNum)
{
	ansNum--;
	size_t ques_size = questionNode->children.size();
	if (ansNum >= 0 && ansNum < ques_size)
	{
		for (size_t i = 0; i < ques_size; i++)
		{
			size_t  index = questionNode->children[i]->content.find("right");
			if (index != std::string::npos)

				/* Make the replacement. */
				questionNode->children[i]->content.replace(index, 5, "");
		}

		questionNode->children[ansNum]->content = "#answer " + std::to_string(ansNum + 1) + " right";
		return true;
	}

	return false;
}

// get right answer by question
// parametr: question node 

size_t TestDocumentManager::getRightAnswer(Node* questionNode)
{
	size_t quesSize = questionNode->children.size();

	for (size_t i = 0; i < quesSize; i++)
	{
		size_t  index = questionNode->children[i]->content.find("right");
		if (index != std::string::npos)

			return i + 1;
	}
	return 0;

}

