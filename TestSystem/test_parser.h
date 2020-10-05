#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

// 
enum class LexemType
{
	Root, Category, Test, Question, Answer
};

struct Lexem
{
	LexemType type;
	std::string content;
	std::string text;

	Lexem(LexemType type_, const std::string& content_)
		: type(type_), content(content_)
	{
	}
};

// tree nodes to build a  document tree
struct Node
{
	Node(LexemType type_, const std::string& content_, const std::string& text_ = "")
		:type(type_), content(content_), text(text_)
	{
	}
	std::vector<Node*> children;

	LexemType type;
	std::string content;
	std::string text;
};


// class  to build a  document tree
//  this will result in pointer to root of tree
class TestParser
{

	// get all lines from file 
	// result -- vector of std::string
	static std::vector<std::string> readAllLines(const std::string& filePath);

	// lexical analizer
	static std::vector<Lexem> parseLexems(const std::vector<std::string>& testContent);
public:

	// syntax analizer
	static Node* parse(const std::string& filePath);
};



