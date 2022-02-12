//
// Created by ferun on 04.02.2022.
//

#ifndef FT_IRC_BOT_IRC_CONNECTION_HPP
#define FT_IRC_BOT_IRC_CONNECTION_HPP

#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include "IRC_Message.hpp"

#include <cstdlib>
#include <sys/poll.h>
#include "sys/socket.h"
#include "string"

#define MAX_IRC_MESSAGE_LEN 512

class IRC_Connection {
private:
	int			_socket_fd;
	sockaddr_in	_socket_info;
	char		_buffer[MAX_IRC_MESSAGE_LEN];
	size_t		_read_amount;
	size_t		_message_end;
	std::string	_command_string;
	pollfd		_pfd;
public:
	IRC_Connection();
	void EstablishConnection(const char *server_host,
							 const char *server_port);
	void SendIRC_Command(std::string &command);
	void GetMessage(IRC_Message &message_to_fill);
	class SoketExeption : public std::exception {
		const char * what() const throw();
	};
	class ConnectExeption : public  std::exception {
		const char * what() const throw();
	};
};


#endif //FT_IRC_BOT_IRC_CONNECTION_HPP
