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
// - the number of mails in mailbox is determined by the largest <ID> in the user's dir

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <regex>
#include <filesystem>
#include <system_error>
#include <set>

#define SAVE_USERS_OPEN_FOR_WRITE_FAILED 1
#define SAVE_USERS_WRITE_FAILED 2
#define SAVE_USERS_OPEN_FOR_APPEND_FAILED 3
#define LOAD_USERS_OPEN_FOR_APPEND_FAILED 4
#define LOAD_USERS_OPEN_FOR_READ_FAILED 5
#define LOAD_USERS_READ_FAILED 6
#define REGISTER_PROMPT_CREATE_DIR_FAILED 7
#define LOAD_MAILBOX_READ_DIR_FAILED 8
#define LOAD_MAILBOX_CHECK_FILE_TYPE_FAILED 9
#define DELETE_USER_FAILED 10
#define DELETE_USER_DELETE_DIR_FAILED 11
#define READ_MAIL_OPEN_FILE_FAILED 12
#define READ_MAIL_READ_FILE_FAILED 13
#define READ_MAIL_INVALID_MAIL 14

using namespace std;

struct Users {
	ofstream outfile;
	map<string, size_t> users_passwords;
};

struct Mailbox {
	size_t mails_count;
	set<size_t> mails;
	string username;
};

struct Mail {
	size_t number;
	string from;
	string subject;
	string content;
};

string get_users_file_path() {
	const string USERS_FILE_PATH = "users.txt";
	return USERS_FILE_PATH;
}

string get_users_file_delim() {
	const string DELIM = ":";
	return DELIM;
}

string get_mail_filepath(Mailbox* mailbox, size_t mail_number) {
	filesystem::path dir(mailbox->username);
	filesystem::path file(to_string(mail_number) + ".txt");
	filesystem::path full_path = dir / file;
	return full_path.string();
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

int delete_user(Users* users, Mailbox* mailbox) {
	if (users->users_passwords.erase(mailbox->username) != 1) {
		cout << "An error has occurred during deleting user." << endl;
		return DELETE_USER_FAILED;
	}

	int save_status_code = save_users(users);

	if (save_status_code != 0) {
		return save_status_code;
	}

	error_code ec;

	if (filesystem::remove_all(mailbox->username, ec) == static_cast<uintmax_t>(-1)) {
		cout << ec.message() << endl;
		return DELETE_USER_DELETE_DIR_FAILED;
	}

	cout << "Successfully deleted user." << endl;

	return 0;
}

int delete_user_prompt(Users* users, Mailbox* mailbox) {
	cout << "--------------------------------" << endl;
	cout << "THIS WILL DELETE USER `" << mailbox->username << "` AND ALL OF ITS EMAILS! THIS CANNOT BE UNDONE! PLEASE ENTER USER'S PASSWORD TO CONFIRM!" << endl;
	cout << "Password: ";
	string password;
	cin >> password;

	size_t hashed_password = hash<string>{}(password);

	if (hashed_password != users->users_passwords.at(mailbox->username)) {
		cout << "Invalid credentials!" << endl;
		return 0;
	}

	return delete_user(users, mailbox);
}

int read_mail(Mailbox* mailbox, size_t mail_number, Mail* mail) {
	ifstream infile(get_mail_filepath(mailbox, mail_number));

	if (!infile) {
		perror("E0012");
		return READ_MAIL_OPEN_FILE_FAILED;
	}

	const int LINES_TO_READ = 3;
	int lines_read = 0;

	string lines[LINES_TO_READ];

	while (getline(infile, lines[lines_read++]) && lines_read < LINES_TO_READ);

	if (infile.bad()) {
		perror("E0013");
		return READ_MAIL_READ_FILE_FAILED;
	}

	if (lines_read < LINES_TO_READ) {
		cout << "An error has occurred during reading mails." << endl;
		return READ_MAIL_INVALID_MAIL;
	}

	string lines_patterns[] = {
		"^From: (.*)$",
		"^Subject: (.*)$",
		"^Content: (.*)$"
	};

	string mail_parts[LINES_TO_READ];

	for (int i = 0; i < LINES_TO_READ; i++) {
		smatch matches;

		if (regex_search(lines[i], matches, regex(lines_patterns[i]))) {
			mail_parts[i] = matches[1];
		} else {
			cout << "An error has occurred during reading mails." << endl;
			return READ_MAIL_INVALID_MAIL;
		}
	}

	mail->from = mail_parts[0];
	mail->subject = mail_parts[1];
	mail->content = mail_parts[2];
	mail->number = mail_number;

	return 0;
}

int list_mailbox(Mailbox* mailbox) {
	for (size_t mail_number : mailbox->mails) {
		Mail mail;

		int result_status = read_mail(mailbox, mail_number, &mail);

		if (result_status != 0) {
			return result_status;
		}

		cout << "(" << mail.number << ") " << mail.subject << endl;
	}

	return 0;
}

int open_mail(Mailbox* mailbox, size_t mail_number) {
	if (mailbox->mails.find(mail_number) == mailbox->mails.end()) {
		cout << "There is no mail with this number." << endl;
		return 0;
	}

	Mail mail;

	int result_status = read_mail(mailbox, mail_number, &mail);

	if (result_status != 0) {
		return result_status;
	}

	cout << "From: " << mail.from << endl;
	cout << "Subject: " << mail.subject << endl;
	cout << "Content: " << mail.content << endl;

	return 0;
}

int open_mail_prompt(Mailbox* mailbox) {
	cout << "Enter mail number: ";
	size_t mail_number;
	cin >> mail_number;

	if (!cin) {
		cout << "Invalid mail number." << endl;
		return 0;
	}

	return open_mail(mailbox, mail_number);
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

size_t get_mail_number(const string filename) {
	smatch matches;

	if (regex_search(filename, matches, regex("^([1-9][0-9]*)\\.txt$"))) {
		stringstream mail_number_stream(matches[1]);
		size_t mail_number;
		mail_number_stream >> mail_number;

		return mail_number;
	} else {
		return 0;
	}
}

int load_mailbox(Mailbox* mailbox, string username) {
	mailbox->username = username;

	error_code ec;

	for (auto dir_entry : filesystem::directory_iterator(username, ec)) {
		if (!dir_entry.is_regular_file(ec)) {
			if (ec) {
				cout << ec.message() << endl;
				return LOAD_MAILBOX_CHECK_FILE_TYPE_FAILED;
			}

			continue;
		}

		size_t mail_number = get_mail_number(dir_entry.path().filename().string());
		
		if (mail_number == 0) {
			// invalid mail number, skipping
			continue;
		}

		mailbox->mails.insert(mail_number);
	}

	if (ec) {
		cout << ec.message() << endl;
		return LOAD_MAILBOX_READ_DIR_FAILED;
	}

	if (mailbox->mails.size() == 0) {
		mailbox->mails_count = 0;
	} else {
		mailbox->mails_count = *(mailbox->mails.rbegin());
	}

	return 0;
}

int mailbox_menu(Users* users, Mailbox* mailbox) {
	while (true) {
		cout << "--------------------------------------------------" << endl;
		cout << "You have " << mailbox->mails_count << " mails. Choose of the following options:" << endl;
		cout << "C - close account" << endl;
		cout << "I - inbox" << endl;
		cout << "L - logout" << endl;
		cout << "O - open" << endl;
		cout << "S - send" << endl;
		cout << "Choose option: ";

		string cmd;
		cin >> cmd;

		int result_status = 0;

		if (cmd == "C") {
			result_status = delete_user_prompt(users, mailbox);
			bool is_user_deleted = (users->users_passwords.find(mailbox->username) == users->users_passwords.end());

			if (result_status == 0 && is_user_deleted) {
				break;
			}
		} else if (cmd == "I") {
			result_status = list_mailbox(mailbox);
		} else if (cmd == "L") {
			break;
		} else if (cmd == "O") {
			result_status = open_mail_prompt(mailbox);
		} else if (cmd == "S") {
			// TODO
		} else {
			cout << "Unrecognized option `" << cmd << "`." << endl;
		}

		if (result_status != 0) {
			return result_status;
		}
	}

	return 0;
}

int enter_mailbox(Users* users, string username) {
	Mailbox mailbox;

	int status_code = load_mailbox(&mailbox, username);

	if (status_code != 0) {
		cout << "An error has occurred during loading mailbox." << endl;
	} else {
		status_code = mailbox_menu(users, &mailbox);
	}

	return status_code;
}

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

int main_menu(Users* users) {
	while (true) {
		cout << "------------------------------------" << endl;
		cout << "Choose one of the following options:" << endl;
		cout << "L - Login" << endl;
		cout << "R - Register" << endl;
		cout << "Q - Quit" << endl;
		cout << "Choose option: ";

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
			cout << "Unrecognized option `" << cmd << "`." << endl;
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
