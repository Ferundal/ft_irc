//
// Created by Radicchio Sarah on 08.01.2022.
//


#include "ConnectSocket.hpp"

ConnectSocket::ConnectSocket(int domain, int type, int protocol) throw (exception)
{
	if ((this->_fd = socket(domain, type, protocol)) == -1) throw exception();
//	if ((setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, (int[]){1}, sizeof(int))) == -1) throw exception();
	binding(); // TODO Возможно стоит поместить в Server
}

//char const * const ConnectSocket::getsockopt() throw (exception)
//{
//	if (setsockopt(this->_fd, SOL_SOCKET, SO_DEBUG, this->_option_val, 1000) == -1)
//		throw exception();
//}

void ConnectSocket::binding(int family, size_t port, const char* ip)
{
	bzero (&this->_addr, sizeof (this->_addr)); // Инициализация памяти нулями
	this->_addr.sin_family = family; // Установка семейства адресов
	this->_addr.sin_port = htons(port); // Установка порта
	if (((int)(this->_addr.sin_addr.s_addr = inet_addr(ip))) == -1)  // Установка адреса
		throw exception();
	if (bind(this->_fd, (struct sockaddr *)&this->_addr, sizeof (struct sockaddr)) == -1)
		throw exception();
}

int ConnectSocket::getfd() const
{
	return _fd;
}

ConnectSocket::~ConnectSocket()
{
//	close(this->_fd);
}
