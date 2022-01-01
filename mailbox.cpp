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
* <file containing implementation for mailbox functionality>
*
*/

#include <string>
#include <regex>
#include <sstream>
#include <filesystem>
#include <system_error>
#include <iostream>
#include "error_codes.h"
#include "mail.h"
#include "mailbox.h"

using namespace std;

size_t get_mail_number(const string filename) {
	smatch matches;

	if (regex_search(filename, matches, regex("^([1-9][0-9]*)\\.txt$"))) {
		stringstream mail_number_stream(matches[1]);
		size_t mail_number;
		mail_number_stream >> mail_number;

		return mail_number;
	}
	else {
		return 0;
	}
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
	}
	else {
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
			result_status = delete_user_prompt(users, mailbox->username);
			bool is_user_deleted = (users->users_passwords.find(mailbox->username) == users->users_passwords.end());

			if (result_status == 0 && is_user_deleted) {
				break;
			}
		}
		else if (cmd == "I") {
			result_status = list_mailbox(mailbox);
		}
		else if (cmd == "L") {
			break;
		}
		else if (cmd == "O") {
			result_status = open_mail_prompt(mailbox);
		}
		else if (cmd == "S") {
			result_status = send_mail_prompt(users, mailbox);
		}
		else {
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
	}
	else {
		status_code = mailbox_menu(users, &mailbox);
	}

	return status_code;
}
