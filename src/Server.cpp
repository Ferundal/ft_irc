//
// Created by ferun on 05.01.2022.
//

#include "Server.hpp"

Server::Server()try : _stream_socket()
{
	if (listen(this->_stream_socket.getsocketfd(), 100) == -1)
		throw exception();
}
catch(exception e)
{
	throw e;
}
