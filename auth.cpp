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
* <file containing authentication functionality>
*
*/

#include <string>
#include <iostream>
#include <system_error>
#include <filesystem>
#include <functional>
#include "error_codes.h"
#include "users.h"
#include "mail.h"
#include "auth.h"

using namespace std;

int login_prompt(Users* users) {
	cout << "----------------" << endl;
	cout << "Enter username: ";
	string username;
	cin >> username;

	cout << "Enter password: ";
	string password;
	cin >> password;

	if (users->users_passwords.find(username) == users->users_passwords.end()) {
		cout << "Invalid credentials!" << endl;
		return 0;
	}

	size_t hashed_password = hash<string>{}(password);

	if (hashed_password != users->users_passwords.at(username)) {
		cout << "Invalid credentials!" << endl;
		return 0;
	}

	return enter_mailbox(users, username);
}

int register_prompt(Users* users) {
	cout << "----------------" << endl;
	cout << "Enter username: ";
	string username;
	cin >> username;

	if (!is_valid_username(username)) {
		cout << "Invalid username! Must have at least 6 characters, only latin letters and digits allowed!" << endl;
		return 0;
	}

	if (users->users_passwords.find(username) != users->users_passwords.end()) {
		cout << "Username already exists!" << endl;
		return 0;
	}

	cout << "Enter password: ";
	string password;
	cin >> password;

	if (!is_valid_password(password)) {
		cout << "Invalid password! Must have at least 6 characters, only latin letters, digits and symbols &*<>?.+- allowed! There must be at least one uppercase letter, at least one lowercase letter, at least one digit and at least one symbol!" << endl;
		return 0;
	}

	size_t hashed_password = hash<string>{}(password);
	users->users_passwords[username] = hashed_password;

	error_code ec;

	if (!filesystem::create_directory(username, ec) && ec) {
		cout << ec.message() << endl;
		return REGISTER_PROMPT_CREATE_DIR_FAILED;
	}

	int save_status_code = save_users(users);

	if (save_status_code != 0) {
		return save_status_code;
	}

	return enter_mailbox(users, username);
}
