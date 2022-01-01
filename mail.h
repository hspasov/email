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
* <header file for mail actions>
*
*/

#ifndef MAIL_H_INCLUDED
#define MAIL_H_INCLUDED

#include <string>
#include "users.h"
#include "mailbox.h"

using namespace std;

struct Mail {
	size_t number;
	string from;
	string subject;
	string content;
};

int open_mail(Mailbox*, size_t);
int read_mail(Mailbox*, size_t, Mail*);
int send_mail(Users*, Mail*, string);
int open_mail_prompt(Mailbox*);
int send_mail_prompt(Users*, Mailbox*);

#endif
