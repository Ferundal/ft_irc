//
// Created by Cetheres Jettie on 12/19/21.
//qw
#include "Server.hpp"
#include "iostream"


int main() {
	try {
		Server sv;
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