#pragma once
#include <string>
#include <iostream>
#include <fstream>

// a class that stores the results of a single test
class TestResult
{
public:
	// default constructor
	TestResult();

	// should be  call this method  for initializing percent & grade 
	void calculate();

// -----------------------------------------------------------------
// To access a private attribute, use public "get" and "set" methods
// -----------------------------------------------------------------
	std::string getCategory() const;

	void setCategory(const std::string& c);

	std::string getNumber() const;

	void setNumber(const std::string& n);

	size_t getAmountQuestions() const;

	void setQuestionsNumber(size_t n);

	size_t getAnsweredQuestions() const;

	void setAnsweredQuestions(size_t n);

	size_t getRightAnswers() const;

	void setRightAnswers(size_t n);

	size_t getPercent() const;

	size_t getGrade() const;

	std::string getLogin() const;

	void setLogin(const std::string& n);




	// stream insertion operator << is used for output object TestResult
	// Overloading output operator for ease of reference 
	//  can simply use operator<< to output the member variables of test result	
	friend std::ostream& operator<<(std::ostream& os, const TestResult& t);

	//  operators <<, >> are  implemented as friend functions.
	friend std::istream& operator>>(std::istream& is, TestResult& t);

private:

	// get  right questions percent
	// result are calculated in percentage of the amount of right answers
	size_t calculatePercent(size_t questions, size_t rightAnswers);

	// get grade 
	// grade  is rated on a 12-point scale
	size_t calculateGrade(size_t questions, size_t rightAnswers);
	

	// category : mathematics, informatic etc. 
	std::string category;
	//  test number 
	std::string test_number;

	//amount of questions
	size_t questions;
	//
	size_t answeredQuestions;

	//amount right questions
	size_t rightAnswers;

	// right questions percent
	size_t percent;

	// tests are rated on a 12-point scale,
	size_t grade;  // 0 - 12

	// user who performed test
	std::string login;
};
