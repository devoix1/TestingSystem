#include "user_manager.h"

 UserManager::UserManager()
{}

UserManager::UserManager(const std::string & filePath_) : filePath(filePath_)
{}

void UserManager::setFilePath(const std::string& filePath_)
{
	filePath = filePath_;
}

std::string UserManager::getFilePath() const
{
	return filePath;
}

// List all users

std::vector<User> UserManager::getAllUsers()
{
	return getAllUsers(filePath);
}

std::vector<User> UserManager::getAllUsers(const std::string& filePath_)
{
	std::vector<User> users;
	// open file for reading
	std::ifstream file(filePath_);
	if (file.is_open()) // Check if a file is open
	{
		std::string line;
		User tmpUser;

		// read file line by line 
		while (std::getline(file, line))
		{
			//  convert line to user object
			std::istringstream iss(line);
			if (!(iss >> tmpUser))
			{
				std::cout << "\n     Error in reading file " << filePath_ << std::endl;
				file.close();
				return users;
			}

			users.push_back(tmpUser);
		}
		file.close();
	}
	else
		std::cout << "\n     Error in opening file " << filePath_ << " for reading." << std::endl;

	return users;
}

// check users file contains login

bool UserManager::findLoginInBase(const std::string& filePath_, const std::string& accLogin)
{
	std::ifstream file(filePath_);  // open file for reading

	if (file.is_open()) // Check if a file is open
	{
		std::string login;

		// read file line by line 
		// &  get first word of line
		while (file >> login)
		{
			if (accLogin == login)
			{
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

bool UserManager::findLoginInBase(const std::string& accLogin)
{
	return findLoginInBase(filePath, accLogin);
}

// function loads user data from file 

bool UserManager::getUserFromBase(const std::string& filePath_, User& acc, bool checkPass)
{
	// open file for reading
	std::ifstream file(filePath_);
	if (file.is_open()) // Check if a file is open
	{
		std::string line;
		User tmpUser;

		// read file line by line 
		while (std::getline(file, line))
		{
			//  convert line to user object
			std::istringstream iss(line);
			if (!(iss >> tmpUser))
			{
				std::cout << "\n     Error in reading file " << filePath_ << std::endl;
				file.close();
				return false;
			}

			if (acc.getLogin() == tmpUser.getLogin())
			{

				if (!checkPass) // if password don't need to check 
				{
					// save user data to variable 
					// and return 
					acc = tmpUser;
					file.close();
					return true;
				}
				else if (acc.getPassword() == tmpUser.getPassword())
				{
					acc = tmpUser; // save user data to variable
					file.close();
					return true;
				}
			}
		}
		file.close();
	}
	else
		std::cout << "\n     Error in opening file " << filePath_ << " for reading." << std::endl;
	return false;
}

bool UserManager::getUserFromBase(User& acc, bool checkPass)
{
	return getUserFromBase(filePath, acc, checkPass);
}

// 

bool UserManager::addUserToBase(const std::string& filePath_, const User& user)
{
	// open file for writing
	// std::ios::app in the constructor for  std::ofstream 
	// tells the program to append to the file  
	std::ofstream file(filePath_, std::ios::out | std::ios::app);
	if (file.is_open()) // Check if a file is open
	{
		//  write user object to the file, use the insertion operator ( << ).
		file << user << std::endl;
		file.close();
	}
	else
	{
		std::cout << "\n     Error in opening file for writing: " << filePath_ << " for writing." << std::endl;
		return false;
	}

	return true;
}

bool UserManager::addUserToBase(const User& user)
{
	return addUserToBase(filePath, user);
}

// fuction edits a line of text that is stored in a  file. 
//  copies everything from the original file and over-write 
// it in a new file, making  edits along the way.  

bool UserManager::deleteUserFromBase(const std::string& filePath_, const std::string& login) {

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
		// login is a first word, therefore 
		// need  find first space 
		// and cut first word	
		size_t spacePos = line.find(" ");
		if (spacePos != std::string::npos)  // if space found
		{
			// cut first word
			std::string firstWord = line.substr(0, spacePos);

			// if login not found 
			//   write line to the new file, use the insertion operator ( << ).
			if (firstWord != login)
				newFile << line << '\n';
		}
	}

	oldFile.close();
	newFile.close();

	// old file should be removed
	std::remove(oldFilePath.c_str());
	return true;
}

bool UserManager::deleteUserFromBase(const std::string& login)
{
	return deleteUserFromBase(filePath, login);
}
