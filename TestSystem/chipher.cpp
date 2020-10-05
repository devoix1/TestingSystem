#include "chipher.h"

// encrypt & decrypt  use the same method  encrypt_decrypt
std::string chipher::encrypt(const std::string& in_string)
{
	return encrypt_decrypt(in_string);
}
 std::string chipher::decrypt(const std::string& in_string)
{
	return encrypt_decrypt(in_string);
}

 // simple xor algorithm
std::string chipher::encrypt_decrypt(const std::string& in_string)
	{
		// Define XOR key 
		// Any character value will work 
		char xorKey = 'P';
		std::string out_string;

		// perform XOR operation of key 
		// with every caracter in string 
		for (size_t i = 0; i < in_string.length(); i++)
		{
			out_string += in_string[i] ^ xorKey;
		}
		return  out_string;
	}
