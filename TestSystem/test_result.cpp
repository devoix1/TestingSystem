#include "test_result.h"

// default constructor
TestResult::TestResult()
// initializing class member variables via a member initializer list 
	: questions(0), answeredQuestions(0), rightAnswers(0), percent(0), grade(0)
{}

// should be  call this method  for initializing percent & grade 
void TestResult::calculate()
{
	percent = calculatePercent(questions, rightAnswers);
	// get grade 
	grade = calculateGrade(questions, rightAnswers);
}

// -----------------------------------------------------------------
// To access a private attribute, use public "get" and "set" methods
// -----------------------------------------------------------------
std::string TestResult::getCategory() const
{
	return category;
}

void TestResult::setCategory(const std::string& c)
{
	category = c;
}

std::string TestResult::getNumber() const
{
	return test_number;
}

void TestResult::setNumber(const std::string& n)
{
	test_number = n;
}

size_t TestResult::getAmountQuestions() const
{
	return questions;
}

void TestResult::setQuestionsNumber(size_t n)
{
	questions = n;
}

size_t TestResult::getAnsweredQuestions() const
{
	return answeredQuestions;
}

void TestResult::setAnsweredQuestions(size_t n)
{
	answeredQuestions= n;
}

size_t TestResult::getRightAnswers() const
{
	return rightAnswers;
}

void TestResult::setRightAnswers(size_t n)
{
	rightAnswers = n;
}

size_t TestResult::getPercent() const
{
	return percent;
}
size_t TestResult::getGrade() const
{
	return grade;
}

std::string TestResult::getLogin() const
{
	return login;
}

void TestResult::setLogin(const std::string& n)
{
	login = n;
}




// stream insertion operator << is used for output object TestResult
// Overloading output operator for ease of reference 
//  can simply use operator<< to output the member variables of test result	
std::ostream& operator<<(std::ostream& os, const TestResult& t)
{
	return os << t.login << " " << t.category << " " << t.test_number 
		<< " " << t.questions << " " << t.answeredQuestions << " " << t.rightAnswers
		<< " " << t.percent << " " << t.grade;
}

//  operators <<, >> are  implemented as friend functions.
std::istream& operator>>(std::istream& is, TestResult& t)
{
	is >> t.login >> t.category >> t.test_number >> t.questions  >> t.answeredQuestions
		>> t.rightAnswers >> t.percent >> t.grade;
	return is;
}

// get  right questions percent
	// result are calculated in percentage of the amount of right answers
size_t TestResult::calculatePercent(size_t questions, size_t rightAnswers)
{
	return (size_t)(rightAnswers * 100 / questions);
}

// get grade 
// grade  is rated on a 12-point scale
size_t TestResult::calculateGrade(size_t questions, size_t rightAnswers)
{
	return (size_t)(rightAnswers * 12 / questions);
}
