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
* <header file for mailbox actions>
*
*/

#ifndef MAILBOX_H_INCLUDED
#define MAILBOX_H_INCLUDED

#include <set>
#include <string>
#include "users.h"

using namespace std;

struct Mailbox {
	size_t mails_count;
	set<size_t> mails;
	string username;
};

int list_mailbox(Mailbox*);
int load_mailbox(Mailbox*, string);
int mailbox_menu(Users*, Mailbox*);
int enter_mailbox(Users*, string);

#endif
