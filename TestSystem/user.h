#pragma once
#include <string>
#include <iostream>

// two roles are used   in the system:
// Administrator and User (student)
enum class Role
{
    Admin, User
};


// a class that stores a user data
class User
{
public:

    // default constructor
    User();

    //----------------------
    // get/set methods 
     //----------------------
    std::string getLogin() const;


    void setLogin(const std::string& c);

    std::string getPassword() const;

    void setPassword(const std::string& c);

    std::string getSurname() const;

    void setSurname(const std::string& c);

    std::string getFirstName() const;

    void setFirstName(const std::string& c);

    std::string getPatronymic() const;

    void setPatronymic(const std::string& c);

    std::string getPhone() const;

    void setPhone(const std::string& c);

    void setRole(Role role);

    bool isAdmin() const;

   
    // overload  << >> operators for User class.
   friend std::ostream& operator<<(std::ostream& os, const User& t);
   friend std::istream& operator>>(std::istream& is, User& t);

   // private members 
private:
    // when a login :  
    std::string login;
    std::string password;
    
    // user must specify surname, first Name, patronymic
    //  phone being registered
    std::string surname, firstName, patronymic;
    std::string phone;

    Role role; // role: user or admin

  
};

bool operator==(const User& lhs, const User& rhs);
bool operator!=(const User& lhs, const User& rhs);


