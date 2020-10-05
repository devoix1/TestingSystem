#include "user.h"

// default constructor
User::User() 
    :  role(Role::User) // The default role for new users
{
}

//----------------------
//   get/set methods 
 //----------------------
std::string User::getLogin() const
{
    return login;
}


void User::setLogin(const std::string& c)
{
    login = c;
}

std::string User::getPassword() const
{
    return password;
}

void User::setPassword(const std::string& c)
{
    password = c;
}

std::string User::getSurname() const
{
    return surname;
}

void User::setSurname(const std::string& c)
{
    surname = c;
}
std::string User::getFirstName() const
{
    return firstName;
}
void User::setFirstName(const std::string& c)
{
    firstName = c;
}
std::string User::getPatronymic() const
{
    return patronymic;
}
void User::setPatronymic(const std::string& c)
{
    patronymic = c;
}
std::string User::getPhone() const
{
    return phone;
}

void User::setPhone(const std::string& c)
{
    phone = c;
}

void User::setRole(Role role_)
{
    role = role_;
}

bool User::isAdmin() const
{
    return role == Role::Admin;
}


bool operator==(const User& lhs, const User& rhs)
{
    return lhs.getLogin() == rhs.getLogin() && lhs.getPassword() == rhs.getPassword()
         && lhs.getSurname() == rhs.getSurname() &&  lhs.getFirstName() == rhs.getFirstName()
          && lhs.getPatronymic() == rhs.getPatronymic()
        && lhs.getPhone() == rhs.getPhone();
}
bool operator!=(const User& lhs, const User& rhs)
{
    // the != operator can also be defined in terms of the == operator,
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const User& t)
{
    // The overloaded operator returns a reference to the original ostream object
    return os << t.login << " " << t.password << " " << t.surname << " " << t.firstName
        << " " << t.patronymic << " " << t.phone;
}

std::istream& operator>>(std::istream& is, User& t)
{
    is >> t.login >> t.password >> t.surname >> t.firstName >> t.patronymic >> t.phone;
    return is;
}
