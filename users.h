/**
*
* Solution to course project # 3
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2021/2022
*
* @author Hristo Spasov
* @idnumber 62278
* @compiler GCC
*
* <header file for users functionality>
*
*/

#ifndef USERS_H_INCLUDED
#define USERS_H_INCLUDED

#include <fstream>
#include <map>
#include <string>

using namespace std;

struct Users {
	ofstream outfile;
	map<string, size_t> users_passwords;
};

int save_users(Users*);
int delete_user(Users*, string);
int load_users(Users*);
int delete_user_prompt(Users*, string);
bool is_valid_username(const string);
bool is_valid_password(const string);

#endif
