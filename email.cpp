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
* <предназначение на файла>
*
*/

// Assumptions:
// - if users.txt contains multiple passwords for a given user, the first is used and the others are deleted
// - an attempt to register a user with existing username fails

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <regex>
#include <filesystem>
#include <system_error>

// TODO define consts for return codes

using namespace std;

struct Users {
	ofstream outfile;
	map<string, size_t> users_passwords;
};

string get_users_file_path() {
	const string USERS_FILE_PATH = "users.txt";
	return USERS_FILE_PATH;
}

string get_users_file_delim() {
	const string DELIM = ":";
	return DELIM;
}

int save_users(Users* users) {
	users->outfile.close();
	users->outfile.open(get_users_file_path(), ofstream::out | ofstream::trunc);

	if (users->outfile.fail()) {
		perror("E0001");
		return 1;
	}

	for (auto it = users->users_passwords.begin(); it != users->users_passwords.end(); it++) {
		string line = it->first + get_users_file_delim() + to_string(it->second);
		users->outfile << line << endl;
	}

	users->outfile.close();

	if (users->outfile.bad()) {
		perror("E0002");
		return 2;
	}

	users->outfile.open(get_users_file_path(), ofstream::app);

	if (users->outfile.fail()) {
		perror("E0003");
		return 3;
	}

	return 0;
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

int load_users(Users* users) {
	users->outfile.open(get_users_file_path(), ofstream::app);

	if (users->outfile.fail()) {
		perror("E0004");
		return 4;
	}

	ifstream infile(get_users_file_path());

	if (!infile) {
		perror("E0005");
		return 5;
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
		cout << username << ", " << password << endl; // TODO delete
	}

	if (infile.bad()) {
		perror("E0006");
		return 6;
	}

	if (is_save_users_required) {
		return save_users(users);
	}

	return 0;
}

int mailbox_prompt(Users* users, string username) {
	return 0;
}

int login_prompt(Users* users) {
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

	return mailbox_prompt(users, username);
}

int register_prompt(Users* users) {
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
		return 7;
	}

	int save_status_code = save_users(users);

	if (save_status_code != 0) {
		return save_status_code;
	}

	return mailbox_prompt(users, username);
}

int main_menu(Users* users) {
	while (true) {
		cout << "Available commands:" << endl;
		cout << "(L) Login" << endl;
		cout << "(R) Register" << endl;
		cout << "(Q) Quit" << endl;
		cout << "Select command: ";

		string cmd;
		cin >> cmd;

		int result_status = 0;

		if (cmd == "L") {
			result_status = login_prompt(users);
		} else if (cmd == "R") {
			result_status = register_prompt(users);
		} else if (cmd == "Q") {
			break;
		} else {
			cout << "Unrecognized command `" << cmd << "`." << endl;
		}

		if (result_status != 0) {
			return result_status;
		}
	}

	return 0;
}

int run_email() {
	Users users;

	int status_code = load_users(&users);

	if (status_code != 0) {
		cout << "An error occurred during loading users" << endl;
	} else {
		status_code = main_menu(&users);
	}

	return status_code;
}

int main() {
	int status_code = run_email();
	
	return status_code;
}
