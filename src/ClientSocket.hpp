//
// Created by Radicchio Sarah on 11.01.2022.
//

#ifndef IRCSERC_CLIENTSOCKET_HPP
#define IRCSERC_CLIENTSOCKET_HPP

#include <netinet/in.h>


struct ClientSocket
{
	int			_fd;
	sockaddr_in	_addr;
	socklen_t	_len;
};


#endif //IRCSERC_CLIENTSOCKET_HPP
