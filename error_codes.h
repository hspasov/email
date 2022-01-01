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
#define SEND_MAIL_CREATE_FILE_FAILED 15
#define SEND_MAIL_WRITE_FILE_FAILED 16

#endif
