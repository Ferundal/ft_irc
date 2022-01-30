#include "ConnectSocket.hpp"

ConnectSocket::ConnectSocket(int domain, int type, int protocol) throw (SocketConfigException) : _fd(-1)
{
	if ((this->_fd = socket(domain, type, protocol)) == -1) throw SocketConfigException("Socket create error");
	if ((setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, (int[]){true}, sizeof(int))) == -1) throw SocketConfigException("Error setsockopt");
}

void ConnectSocket::binding(int family, int port, in_addr_t ip)
{
	bzero (&this->_addr, sizeof (this->_addr)); // Инициализация памяти нулями
	this->_addr.sin_family = family; // Установка семейства адресов
	this->_addr.sin_addr.s_addr = ip; //Установка ip, которое мы хотим принять
	this->_addr.sin_port = htons(port); // Установка порта
	if (bind(this->_fd, (struct sockaddr*)&this->_addr, sizeof (this->_addr)) == -1)
		throw SocketConfigException("Binding error");
}

int ConnectSocket::getfd() const
{
	return _fd;
}

ConnectSocket::~ConnectSocket()
{
	if (this->_fd != -1)
	{
		close(this->_fd);
	}
}
