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
* <file containing implementation for users functionality>
*
*/

#include <string>
#include <regex>
#include <fstream>
#include <cstdio>
#include <system_error>
#include <filesystem>
#include <sstream>
#include <functional>
#include <iostream>
#include "error_codes.h"
#include "users.h"

using namespace std;

string get_users_file_path() {
	const string USERS_FILE_PATH = "users.txt";
	return USERS_FILE_PATH;
}

string get_users_file_delim() {
	const string DELIM = ":";
	return DELIM;
}

bool is_valid_username(const string username) {
	return regex_match(username, regex("^[a-zA-Z0-9]{6,}$"));
}

bool is_valid_password(const string password) {
	return regex_search(password, regex("[A-Z]")) &&
		regex_search(password, regex("[a-z]")) &&
		regex_search(password, regex("[0-9]")) &&
		regex_search(password, regex("[&*<>?.+-]")) &&
		regex_match(password, regex("^[a-zA-Z0-9&*<>?.+-]{6,}$"));
}

int save_users(Users* users) {
	users->outfile.close();
	users->outfile.open(get_users_file_path(), ofstream::out | ofstream::trunc);

	if (users->outfile.fail()) {
		perror("E0001");
		return SAVE_USERS_OPEN_FOR_WRITE_FAILED;
	}

	for (auto entry : users->users_passwords) {
		string line = entry.first + get_users_file_delim() + to_string(entry.second);
		users->outfile << line << endl;
	}

	users->outfile.close();

	if (users->outfile.bad()) {
		perror("E0002");
		return SAVE_USERS_WRITE_FAILED;
	}

	users->outfile.open(get_users_file_path(), ofstream::app);

	if (users->outfile.fail()) {
		perror("E0003");
		return SAVE_USERS_OPEN_FOR_APPEND_FAILED;
	}

	return 0;
}

int delete_user(Users* users, string username) {
	if (users->users_passwords.erase(username) != 1) {
		cout << "An error has occurred during deleting user." << endl;
		return DELETE_USER_FAILED;
	}

	int save_status_code = save_users(users);

	if (save_status_code != 0) {
		return save_status_code;
	}

	error_code ec;

	if (filesystem::remove_all(username, ec) == static_cast<uintmax_t>(-1)) {
		cout << ec.message() << endl;
		return DELETE_USER_DELETE_DIR_FAILED;
	}

	cout << "Successfully deleted user." << endl;

	return 0;
}

int load_users(Users* users) {
	users->outfile.open(get_users_file_path(), ofstream::app);

	if (users->outfile.fail()) {
		perror("E0004");
		return LOAD_USERS_OPEN_FOR_APPEND_FAILED;
	}

	ifstream infile(get_users_file_path());

	if (!infile) {
		perror("E0005");
		return LOAD_USERS_OPEN_FOR_READ_FAILED;
	}

	bool is_save_users_required = false;
	string line;

	while (getline(infile, line)) {
		const size_t delimiter_idx = line.rfind(get_users_file_delim());

		bool invalid_entry = (delimiter_idx == string::npos);

		if (invalid_entry) {
			is_save_users_required = true;
			continue;
		}

		const string username = line.substr(0, delimiter_idx);
		const string password_str = line.substr(delimiter_idx + 1, string::npos);

		stringstream password_stream(password_str);
		size_t password;
		password_stream >> password;

		bool entry_exists = (users->users_passwords.find(username) != users->users_passwords.end());
		bool is_invalid_username = !is_valid_username(username);
		bool is_invalid_password = (to_string(password) != password_str);

		if (entry_exists || is_invalid_username || is_invalid_password) {
			is_save_users_required = true;
			continue;
		}

		users->users_passwords[username] = password;
	}

	if (infile.bad()) {
		perror("E0006");
		return LOAD_USERS_READ_FAILED;
	}

	if (is_save_users_required) {
		return save_users(users);
	}

	return 0;
}

int delete_user_prompt(Users* users, string username) {
	cout << "--------------------------------" << endl;
	cout << "THIS WILL DELETE USER `" << username << "` AND ALL OF ITS EMAILS! THIS CANNOT BE UNDONE! PLEASE ENTER USER'S PASSWORD TO CONFIRM!" << endl;
	cout << "Password: ";
	string password;
	cin >> password;

	size_t hashed_password = hash<string>{}(password);

	if (hashed_password != users->users_passwords.at(username)) {
		cout << "Invalid credentials!" << endl;
		return 0;
	}

	return delete_user(users, username);
}
