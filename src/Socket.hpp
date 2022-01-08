//
// Created by Radicchio Sarah on 08.01.2022.
//

#ifndef IRCSERC_SOCKET_HPP
#define IRCSERC_SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include "exception"

using namespace std;


class Socket
{
public:
	explicit Socket(int domain=PF_UNIX, int type=SOCK_STREAM, int protocol=0) throw (exception);
	//char* getsockopt();

private:
	int ret_status;

};


#endif //IRCSERC_SOCKET_HPP
