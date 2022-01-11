//
// Created by Cetheres Jettie on 12/19/21.
//

#include "Server.hpp"

int main() {
	try {
		Server sv;
		while (true)
		{
			sv.listening();
		}
	}
	catch (const exception& e)
	{
		e.what();
	}
	return (0);
}