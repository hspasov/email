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

#include <string>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

struct Users {
	ofstream outfile;
	map<string, string> users_passwords;
};

string get_users_file_path () {
	const string USERS_FILE_PATH = "users.txt";
	return USERS_FILE_PATH;
}

string get_users_file_delim() {
	const string DELIM = ":";
	return DELIM;
}

Users* save_users(Users* users) {
	users->outfile.close();
	users->outfile.open(get_users_file_path(), ofstream::out | ofstream::trunc);

	if (users->outfile.fail()) {
		perror("E0001");
		return nullptr;
	}

	for (auto it = users->users_passwords.begin(); it != users->users_passwords.end(); it++) {
		string line = it->first + get_users_file_delim() + it->second;
		users->outfile << line << endl;
	}

	users->outfile.close();

	if (users->outfile.bad()) {
		perror("E0002");
		return nullptr;
	}

	users->outfile.open(get_users_file_path(), ofstream::app);

	if (users->outfile.fail()) {
		perror("E0003");
		return nullptr;
	}

	return users;
}

Users* load_users() {
	Users* users = new Users();

	users->outfile.open(get_users_file_path(), ofstream::app);

	if (users->outfile.fail()) {
		perror("E0004");
		return nullptr;
	}

	ifstream infile(get_users_file_path());

	if (!infile) {
		perror("E0005");
		return nullptr;
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
		const string password = line.substr(delimiter_idx + 1, string::npos);

		bool entry_exists = (users->users_passwords.find(username) != users->users_passwords.end());

		if (entry_exists) {
			is_save_users_required = true;
			continue;
		}

		users->users_passwords[username] = password;
		cout << username << ", " << password << endl;
	}

	if (infile.bad()) {
		perror("E0006");
		return nullptr;
	}

	if (is_save_users_required) {
		if(!save_users(users)) {
			return nullptr;
		}
	}

	return users;
}

void unload_users(Users* users) {
	delete users;
}

void main_menu(Users* users) {
	while (true) {
		cout << "Available commands:" << endl;
		cout << "(L) Login" << endl;
		cout << "(R) Register" << endl;
		cout << "(Q) Quit" << endl;
		cout << "Select command: ";

		string cmd;
		cin >> cmd;

		if (cmd == "L") {

		} else if (cmd == "R") {

		} else if (cmd == "Q") {
			break;
		} else {
			cout << "Unrecognized command `" << cmd << "`." << endl;
		}
	}
}

void run_email() {
	Users* users = load_users();

	if (users == nullptr) {
		cout << "An error occurred during loading users" << endl;
	} else {
		main_menu(users);
	}

	unload_users(users);
}

int main() {
	run_email();
	
	return 0;
}
