#pragma once
#include <string>

//  cipher is an class for performing encryption or decryption
class chipher 
{
public:
	static std::string encrypt(const std::string& in_string);

	static std::string decrypt(const std::string& in_string);


private:  // xor  encrypt/decrypt method 
	static std::string encrypt_decrypt(const std::string& in_string);
};

