#include "Bot.hpp"
#include "IRC_Connection.hpp"

#include <iostream>

int check_arg(int argc,const char **argv) {
	(void)argv;
	if (argc < 5 || argc > 6) {
		std::cout << "Wrong param amount. Expected: <bot name> <bot password> <server host> <server port> [<server password>]\n" <<std::endl;
		return (1);
	}
	return (0);
}

int main(int argc,const char **argv) {
	if (check_arg(argc, argv) != 0)
		return (1);
	try {
		IRC_Connection connection;
		connection.EstablishConnection(argv[3], argv[4]);
		Bot bot(argv[1], argv[2], connection);
		if (argc < 6)
			bot.Login(NULL);
		else
			bot.Login(argv[5]);
		bot.Start();
	}
	catch (std::exception &exception) {
		std::cout << exception.what() << std::endl;
		return (2);
	}
}