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
#include "exception"

using namespace std;


class Socket
{
public:
	explicit Socket(int domain=AF_INET, int type=SOCK_STREAM, int protocol=0) throw (exception);
	char const * const		getsockopt() throw (exception);
	int getsocketfd();
	void binding(int family=AF_INET, size_t port=800, const char* ip="192.168.1.0");

	~Socket();

private:
	int			_socket_fd;
	sockaddr_in	_socket_addr;
	char*		_option_val;
};


#endif //IRCSERC_SOCKET_HPP
