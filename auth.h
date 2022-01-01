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
* <header file for authentication functionality>
*
*/

#ifndef AUTH_H_INCLUDED
#define AUTH_H_INCLUDED

#include "users.h"

using namespace std;

int login_prompt(Users*);
int register_prompt(Users*);

#endif
