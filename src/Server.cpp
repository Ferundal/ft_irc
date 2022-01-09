//
// Created by ferun on 05.01.2022.
//


#include "Server.hpp"

Server::Server()try : _stream_socket()
{
	listen(this->_stream_socket.getsocketfd(), 5); //  n - максимальная длина, до которой может расти очередь ожидающих соединений
}
catch(exception e)
{
	throw e;
}

void Server::listening()
{
	pollfd pfd;
	char buf[10];

	int new_socket_fd;
	sockaddr connected_socked;
	socklen_t socket_len;


	pfd.fd = this->_stream_socket.getsocketfd();
	pfd.events = POLLIN;
	poll(&pfd, 1, -1);
	if ((pfd.revents & POLLIN) == POLLIN)
	{
		new_socket_fd = accept(this->_stream_socket.getsocketfd(), &connected_socked, &socket_len);
		read(new_socket_fd, buf, 10);
		write(1, buf, 10);
	}
}