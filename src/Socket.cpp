 //
// Created by Radicchio Sarah on 08.01.2022.
//


#include "Socket.hpp"

Socket::Socket(int domain, int type, int protocol) throw (exception)
{
	if ((this->_socket_fd = socket(domain, type, protocol)) == -1) throw exception();
}

//char const * const Socket::getsockopt() throw (exception)
//{
//	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_DEBUG, this->_option_val, 1000) == -1)
//		throw exception();
//}

void Socket::binding(int family, size_t port, const char* ip)
{
	bzero (&this->_socket_addr, sizeof (this->_socket_addr)); // Инициализировать структуру
	this->_socket_addr.sin_family = family; // Установить семейство адресов
	this->_socket_addr.sin_port = htons(port); // Установить порт
	if (((int)(this->_socket_addr.sin_addr.s_addr = inet_addr(ip))) == -1)  // Установить адрес
		throw exception();
	if (bind(this->_socket_fd, (struct sockaddr *)&this->_socket_addr, sizeof (struct sockaddr)) == -1)
			throw exception();
}

int Socket::getsocketfd()
{
	return _socket_fd;
}

Socket::~Socket()
{
	close(this->_socket_fd);
}
