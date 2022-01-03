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
* <file with error codes>
*
*/

#ifndef ERROR_CODES_H_INCLUDED
#define ERROR_CODES_H_INCLUDED

const int SAVE_USERS_OPEN_FOR_WRITE_FAILED = 1;
const int SAVE_USERS_WRITE_FAILED = 2;
const int SAVE_USERS_OPEN_FOR_APPEND_FAILED = 3;
const int LOAD_USERS_OPEN_FOR_APPEND_FAILED = 4;
const int LOAD_USERS_OPEN_FOR_READ_FAILED = 5;
const int LOAD_USERS_READ_FAILED = 6;
const int REGISTER_PROMPT_CREATE_DIR_FAILED = 7;
const int LOAD_MAILBOX_READ_DIR_FAILED = 8;
const int LOAD_MAILBOX_CHECK_FILE_TYPE_FAILED = 9;
const int DELETE_USER_FAILED = 10;
const int DELETE_USER_DELETE_DIR_FAILED = 11;
const int READ_MAIL_OPEN_FILE_FAILED = 12;
const int READ_MAIL_READ_FILE_FAILED = 13;
const int READ_MAIL_INVALID_MAIL = 14;
const int SEND_MAIL_CREATE_FILE_FAILED = 15;
const int SEND_MAIL_WRITE_FILE_FAILED = 16;

#endif
