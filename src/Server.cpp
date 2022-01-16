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

void Server::communication()
{

}

void Server::listening()
{
	if (poll(_pfd.data(), _pfd.size(), -1) == -1) throw exception();
	if (_pfd[0].revents & POLLIN)
	{
		// Новый пользователь
		_clnt_sockets.push_back(ClientSocket());
		_clnt_sockets.back()._fd = accept(_cnct_socket.getfd(), (sockaddr*)&_clnt_sockets.back()._addr, &_clnt_sockets.back()._len);
		_pfd.push_back(pollfd());
		bzero(&_pfd.back(), sizeof (_pfd.back()));
		_pfd.back().fd = _clnt_sockets.back()._fd;
		_pfd.back().events = POLLIN | POLLERR | POLLHUP; // По умолчанию у клиентских сокетов запись открыта, POLLOUT не нужно смотреть
		_pfd[0].revents = 0;
		if (poll(_pfd.data(), _pfd.size(), -1) == -1) throw exception();
	}
	for(vector<pollfd>::iterator it = ++_pfd.begin(); it < _pfd.end(); ++it)
	{
		if(it->revents & POLLHUP)
		{
			// terminate();
			this->_clnt_sockets.erase(findSocketIter(it->fd));
			this->_pfd.erase(it);
			return;
		}
		else if(it->revents & POLLIN )
		{
			//Пользователь на сокете ждет ответ
			int r_len;
			ClientSocket& sckt = *findSocketIter(it->fd);

			if ((r_len = recv(it->fd, const_cast<char*>( sckt._msg_buff.data()), 1024, 0)) == 0)
			{
				for(int i = 0; i < 5; ++i) // Пиздец
				{
					send(it->fd, (char[1]){0}, 1, MSG_NOSIGNAL);
					if (errno & EPIPE)
					{
						this->_clnt_sockets.erase(findSocketIter(it->fd));
						this->_pfd.erase(it);
						errno = 0;
						return;
					}
				}
			}
			it->revents = 0;
//			write(1, (char[]){it->fd + 48, '\n'}, 2);
//			write(1, sckt._msg_buff.c_str(), r_len);

//			if (chekMsgEnding(sckt._msg_buff))
			this->_parser.stringParser(sckt._msg_buff);
			send(it->fd, sckt._msg_buff.data(), r_len, MSG_NOSIGNAL);
		}
	}
}

bool Server::chekMsgEnding(string& str)
{
	if (str.find("\r\n") == str.rfind("\r\n"))
		return true;
	return false;
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

Server::~Server() {}
