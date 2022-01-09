//
// Created by Radicchio Sarah on 08.01.2022.
//

#ifndef IRCSERC_SOCKET_HPP
#define IRCSERC_SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/poll.h>
#include "exception"

#define PORT	5001
#define IP		"192.168.1.25"

using namespace std;


class Socket
{
public:
	explicit Socket(int domain=AF_INET, int type=SOCK_STREAM, int protocol=0) throw (exception);
//	char const * const		getsockopt() throw (exception);
	int getsocketfd();
	void binding(int family=AF_INET, size_t port=PORT, const char* ip=IP);

	~Socket();

private:
	int			_socket_fd;
	sockaddr_in	_socket_addr;
};


#endif //IRCSERC_SOCKET_HPP
