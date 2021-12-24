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

#include <string>
#include <fstream>
#include <iostream>
#include <map>

struct Users {
	std::ofstream* outfile;
	std::map<std::string, std::string> usersPasswords;
};

void load_users(struct Users* users) {
	const std::string USERS_FILE_PATH = "users.txt";

	users->outfile = new std::ofstream(USERS_FILE_PATH, std::ofstream::app);
	std::ifstream infile(USERS_FILE_PATH);

	std::string line;

	while (infile >> line) {
		// TODO handle errors
		const std::string DELIMITER = ":";
		const size_t delimiter_idx = line.rfind(DELIMITER);
		const std::string username = line.substr(0, delimiter_idx);
		const std::string password = line.substr(delimiter_idx + 1, std::string::npos);

		std::cout << username << "," << password << std::endl;
	}

}

void release_users(struct Users* users) {
	delete users->outfile;
}

int main () {
	struct Users users;

	load_users(&users);

	release_users(&users);

	return 0;
}
