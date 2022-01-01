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
* <file containing implementation for mail actions>
*
*/

#include <cstdio>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include "error_codes.h"
#include "mail.h"
#include "mailbox.h"
#include "users.h"

using namespace std;

string get_mail_filepath(Mailbox* mailbox, size_t mail_number) {
	filesystem::path dir(mailbox->username);
	filesystem::path file(to_string(mail_number) + ".txt");
	filesystem::path full_path = dir / file;
	return full_path.string();
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

	mail->from = lines[0];
	mail->subject = lines[1];
	mail->content = lines[2];
	mail->number = mail_number;

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

int send_mail(Users* users, Mail* mail, string recipient) {
	if (users->users_passwords.find(recipient) == users->users_passwords.end()) {
		cout << "Recipient `" << recipient << "` does not exist." << endl;
		return 0;
	}

	Mailbox recipient_mailbox;

	int status_code = load_mailbox(&recipient_mailbox, recipient);

	if (status_code != 0) {
		cout << "An error has occurred during sending mail." << endl;
		return status_code;
	}

	mail->number = recipient_mailbox.mails_count + 1;

	ofstream outfile(get_mail_filepath(&recipient_mailbox, mail->number), ofstream::app);

	if (outfile.fail()) {
		perror("E0015");
		return SEND_MAIL_CREATE_FILE_FAILED;
	}

	outfile << mail->from << endl;
	outfile << mail->subject << endl;
	outfile << mail->content << endl;

	outfile.close();

	if (outfile.bad()) {
		perror("E0016");
		return SEND_MAIL_WRITE_FILE_FAILED;
	}

	cout << "Successfully sent email." << endl;

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

int send_mail_prompt(Users* users, Mailbox* sender_mailbox) {
	cout << "To: ";
	string recipient;
	cin >> recipient;

	// throw away the rest of the line
	char c;
	while (cin.get(c) && c != '\n');

	Mail mail;
	mail.from = sender_mailbox->username;

	cout << "Subject: ";
	getline(cin, mail.subject);

	cout << "Content: ";
	getline(cin, mail.content);

	return send_mail(users, &mail, recipient);
}
