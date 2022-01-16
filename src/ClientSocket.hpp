//
// Created by Radicchio Sarah on 11.01.2022.
//

#ifndef IRCSERC_CLIENTSOCKET_HPP
#define IRCSERC_CLIENTSOCKET_HPP

#include <netinet/in.h>
#include <unistd.h>
#include "string"

#define MSG_BUFF 1024

using namespace std;

struct ClientSocket
{
	int			_fd;
	sockaddr_in	_addr;
	socklen_t	_len;
	string		_msg_buff;

	ClientSocket();
	~ClientSocket();
};


#endif //IRCSERC_CLIENTSOCKET_HPP
