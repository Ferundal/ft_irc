//
// Created by ferun on 05.01.2022.
//

#include "Server.hpp"

Server::Server(int n_connect) try : _cnct_socket()
{
	listen(_cnct_socket.getfd(), n_connect); //  n_connect - максимальная длина, до которой может расти очередь ожидающих соединений
	// Внесение данных первого элемента ConnetSocket в pfd
	{
		_pfd.push_back(pollfd());
		bzero(&_pfd[0], sizeof (_pfd[0]));
		_pfd[0].fd = _cnct_socket.getfd();
		/*
		 * POLLIN      0x0001     Можно считывать данные
		 * POLLPRI     0x0002     Есть срочные данные
		 * POLLOUT     0x0004     Запись не будет блокирована
		 * POLLERR     0x0008     Произошла ошибка
		 * POLLHUP     0x0010     "Положили трубку"
		 * POLLNVAL    0x0020     Неверный запрос: fd не открыт
		 */
		_pfd[0].events = POLLIN;

	}
}
catch(exception& e)
{
	throw e;
}

void Server::grabConnection()
{
	if (poll(_pfd.data(), _pfd.size(), -1) == -1) throw exception();
	if (_pfd[0].revents & POLLIN)
		// Новый пользователь
		addNewClientSocket();
	if (poll(_pfd.data(), _pfd.size(), -1) == -1) throw exception();
	for(vector<pollfd>::iterator it = ++_pfd.begin(); it < _pfd.end(); ++it)
	{
		if(it->revents & POLLHUP)
		{
			//Пользователь отсоединился
			deleteClientSocket(it);
			return;
		}
		else if(it->revents & POLLIN )
		{
			//Пользователь  на сокете ждет ответ
			readCommand(it);
		}
	}
}

void	Server::readCommand(vector<pollfd>::iterator it)
{
	bool	r_frst_flag = false;
	int		r_len;
	char	r_buf[2] = {0,};
	ClientSocket& sckt = *findSocketIter(it->fd);

	while (true)
	{
		r_len = recv(it->fd, &r_buf, 1, 0);
		if (((r_len == 0) && (r_frst_flag == false)))
		{
		 	deleteClientSocket(it);
			return;
		}else
		{
			r_frst_flag = true;
		}
		sckt._msg_buff.append(r_buf);
		if (sckt._msg_buff.find("\r\n") != string::npos)
			break;
	}
	if (sckt._msg_buff.find("\r\n") != string::npos)
	{
		sckt._msg_buff.erase(sckt._msg_buff.size() - 2, 2);
		this->_parser.stringParser(sckt);
	}
	it->revents = 0;
}

void	Server::addNewClientSocket()
{
	ClientSocket new_client;
	new_client._fd = accept(_cnct_socket.getfd(), (sockaddr*)&new_client._addr, &new_client._len);
	new_client._usr_ptr = &this->_user_bd.CreateNewUser(new_client._fd);
	_clnt_sockets.push_back(new_client);

	pollfd new_pfd;
	bzero(&new_pfd, sizeof (new_pfd));
	new_pfd.fd = new_client._fd;
	new_pfd.events = POLLIN | POLLERR | POLLHUP; // По умолчанию у клиентских сокетов запись открыта, POLLOUT не нужно смотреть
	new_pfd.revents = 0;
	_pfd.push_back(new_pfd);
}

void 	Server::deleteClientSocket(vector<pollfd>::iterator& it)
{
	this->_clnt_sockets.erase(findSocketIter(it->fd));
	close(it->fd);
	this->_pfd.erase(it);
}

int		Server::findOpenFD()
{
	int i = 5;
	for(vector<ClientSocket>::iterator it = _clnt_sockets.begin(); it < _clnt_sockets.end(); ++it, ++i)
	{
		if (it->_fd != i)
			return i;
	}
	return i;
}

vector<ClientSocket>::iterator Server::findSocketIter(int fd)
{
	for(vector<ClientSocket>::iterator it = _clnt_sockets.begin(); it < _clnt_sockets.end(); ++it)
	{
		if(it->_fd == fd)
			return (it);
	}
	throw exception();
}

Server::~Server()
{

}
