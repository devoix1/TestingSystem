
#include "test_system.h"

int main()
{
	TestSystem tsystem("users.txt", "tests.txt", "results.txt");

	tsystem.mainMenu(); 
}


