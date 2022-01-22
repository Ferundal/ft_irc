//
// Created by Radicchio Sarah on 08.01.2022.
//


#include "ConnectSocket.hpp"
#include "iostream"

ConnectSocket::ConnectSocket(int domain, int type, int protocol) throw (exception) : _fd(-1)
{
	cout << "Cteate connection socket" << endl;
	if ((this->_fd = socket(domain, type, protocol)) == -1) throw exception();
	if ((setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, (int[]){true}, sizeof(int))) == -1) throw exception();
	binding(); // TODO Возможно стоит поместить в Server
}

void ConnectSocket::binding(int family, int port, in_addr_t ip)
{
	bzero (&this->_addr, sizeof (this->_addr)); // Инициализация памяти нулями
	this->_addr.sin_family = family; // Установка семейства адресов
	this->_addr.sin_addr.s_addr = ip;
	this->_addr.sin_port = htons(port); // Установка порта
	if (bind(this->_fd, (struct sockaddr*)&this->_addr, sizeof (this->_addr)) == -1)
		throw exception();
}

int ConnectSocket::getfd() const
{
	return _fd;
}

ConnectSocket::~ConnectSocket()
{
	if (this->_fd != -1)
	{
		cout << "Close fd " << this->_fd;
		close(this->_fd);
	}
}
