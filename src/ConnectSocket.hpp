//
// Created by Radicchio Sarah on 08.01.2022.
//

#ifndef IRCSERC_CONNECTSOCKET_HPP
#define IRCSERC_CONNECTSOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sys/poll.h>
#include "exception"

#define PORT	50001
#define IP		"127.0.0.1"

using namespace std;


class ConnectSocket
{
public:
	/*
	 * protocol=0 - по умолчанию - в соответствии с domain
	 */
	explicit ConnectSocket(int domain=AF_INET, int type=SOCK_STREAM, int protocol=0) throw (exception);
//	char const * const		getsockopt() throw (exception);
	virtual int getfd() const;
	void binding(int family=AF_INET, size_t port=PORT, const char* ip=IP);

	~ConnectSocket();
private:
	int			_fd;
	sockaddr_in	_addr;
};


#endif //IRCSERC_CONNECTSOCKET_HPP
