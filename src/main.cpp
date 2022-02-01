//
// Created by Cetheres Jettie on 12/19/21.
//qw
#include "Server.hpp"
#include "iostream"

int check_arg(int argc, char **argv) {
	(void)argv;
	if (argc != 3) {
		std::cout << "Wrong param amount. Expected two: port and password" <<std::endl;
		return (1);
	}
	return (0);
}

int main(int argc, char **argv) {

	(void)argc;
	(void)argv;
	if (check_arg(argc, argv) != 0)
		return (1);
	try {
		Server sv(argv[1],argv[2]);
		while (true)
		{
			sv.grabConnection();
		}
	}
	catch (const exception& e)
	{
		std::cout << e.what() << std::endl;
	}
	return (0);
}