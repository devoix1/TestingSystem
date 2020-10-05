#include "test_parser.h"

// get all lines from file 
// result -- vector of std::string

std::vector<std::string> TestParser::readAllLines(const std::string& filePath)
{
	// open file for reading 
	std::ifstream file(filePath);
	std::vector<std::string> lines;

	if (!file.is_open())
	{
		std::cout << "\n     Error in opening file: " << filePath << std::endl;
		return lines;
	}
	std::string line;

	while (std::getline(file, line))
	{
		lines.push_back(line);
	}

	return lines;
}

// lexical analizer

std::vector<Lexem> TestParser::parseLexems(const std::vector<std::string>& testContent)
{
	std::string text = "";
	std::vector<Lexem> lexems;

	// 
	for (auto& line : testContent)
	{
		if (line.find("#category") != std::string::npos)
			lexems.push_back(Lexem(LexemType::Category, line));
		else if (line.find("#test") != std::string::npos)
			lexems.push_back(Lexem(LexemType::Test, line));
		else if (line.find("#question") != std::string::npos)
			lexems.push_back(Lexem(LexemType::Question, line));
		else if (line.find("#answer") != std::string::npos)
			lexems.push_back(Lexem(LexemType::Answer, line));
		else  // simple text
			lexems.back().text += /*"\n" +*/line;
	}
	return lexems;
}

// syntax analizer

Node* TestParser::parse(const std::string& filePath)
{
	//get lexems
	auto lexems = parseLexems(readAllLines(filePath));
	//check
	if (lexems.size() < 1)
	{
		//throw std::exception( "Empty document");
		return nullptr;
	}
	if (lexems[0].type != LexemType::Category)
	{
		throw std::exception("Document should start with \"#category\"");
	}

	//build node tree
	auto doc = new Node(LexemType::Root, "Root");

	for (auto& lexem : lexems)
		switch (lexem.type)
		{
		case LexemType::Category:
		{
			auto node = new Node(lexem.type, lexem.content);
			doc->children.push_back(node);
			break;
		}
		case LexemType::Test:
		{
			auto node = new Node(lexem.type, lexem.content);
			doc->children.back()->
				children.push_back(node);
			break;
		}
		case LexemType::Question:
		{	auto node = new Node(lexem.type, lexem.content, lexem.text);
		//
		doc->children.back()->
			children.back()->
			children.push_back(node);

		break;
		}
		case LexemType::Answer:
		{
			auto node = new Node(lexem.type, lexem.content, lexem.text);
			doc->children.back()->
				children.back()->
				children.back()->
				children.push_back(node);
			break;
		}
		}
	return doc;  // return root element of tree 
}

