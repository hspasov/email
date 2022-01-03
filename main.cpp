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
* <file containing main function and main menu>
*
*/

// Assumptions:
// - if users.txt contains multiple passwords for a given user, the first is used and the others are deleted
// - an attempt to register a user with existing username fails
// - the number of mails in mailbox is determined by the largest <ID> in the user's dir

#include <string>
#include <iostream>
#include "users.h"
#include "auth.h"

using namespace std;

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
		}
		else if (cmd == "R") {
			result_status = register_prompt(users);
		}
		else if (cmd == "Q") {
			break;
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

int run_email() {
	Users users;

	int status_code = load_users(&users);

	if (status_code != 0) {
		cout << "An error occurred during loading users." << endl;
	}
	else {
		status_code = main_menu(&users);
	}

	return status_code;
}

int main() {
	return run_email();
}
