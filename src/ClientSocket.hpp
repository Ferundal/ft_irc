//
// Created by Radicchio Sarah on 11.01.2022.
//

#ifndef IRCSERC_CLIENTSOCKET_HPP
#define IRCSERC_CLIENTSOCKET_HPP

#include <netinet/ip.h>
#include <unistd.h>
#include "string"
#include "User.hpp"

#define MSG_BUFF 1024

using namespace std;

struct ClientSocket
{
	int			_fd;
	sockaddr_in	_addr;
	socklen_t	_len;
	string		_msg_buff;
	User*		_usr_ptr;

	ClientSocket();
	~ClientSocket();
};


#endif //IRCSERC_CLIENTSOCKET_HPP
