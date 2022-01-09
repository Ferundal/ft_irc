//
// Created by ferun on 05.01.2022.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "Socket.hpp"
#include "exception"
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

class Server {
public:
	Server();
private:
	Socket _stream_socket;
};


#endif //FT_IRC_SERVER_HPP
