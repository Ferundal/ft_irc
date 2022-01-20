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

			//Пользователь на сокете ждет ответ
			int		r_len;
			char	r_buf[2] = {0,};
			ClientSocket& sckt = *findSocketIter(it->fd);

			while (true)
			{
				r_len = recv(it->fd, &r_buf, 1, 0);
				if (r_len == 0)// && checkDisconnect(it))
					break;
				sckt._msg_buff.append(r_buf);
				if (sckt._msg_buff.find("\r\n") != string::npos)
					break;
			}
			if (sckt._msg_buff.find("\r\n") != string::npos)
			{
//				cout << "###########" << endl;
				cout << sckt._msg_buff.size() << ") " << sckt._msg_buff.data() << endl;
				sckt._msg_buff.erase(sckt._msg_buff.size() - 2, 2);
				this->_parser.stringParser(sckt._msg_buff);
//				sckt._msg_buff.clear(); //DEBUGGING
				//				write(1, "\nCleaned\n", 9); //DEBUGGING
				//				send(it->fd, sckt._msg_buff.data(), r_len, MSG_NOSIGNAL);
				//				write(1, (char[]){it->fd + 48, '\n'}, 2); //DEBUGGING
			}
			it->revents = 0;
		}
	}
}

bool	Server::checkDisconnect(vector<pollfd>::iterator& it)
{
	for(int i = 0; i < 5; ++i) // Пиздец
	{
		send(it->fd, (char[1]){0}, 1, MSG_NOSIGNAL);
		if (errno & EPIPE)
		{
			deleteClientSocket(it);
			errno = 0;
			return true;
		}
	}
	return false;
}

void	Server::addNewClientSocket()
{
	_clnt_sockets.push_back(ClientSocket());
	ClientSocket& clnt_s = _clnt_sockets.back();
	clnt_s._usr_ptr = &this->_user_bd.CreateNewUser();
	clnt_s._fd = accept(_cnct_socket.getfd(), (sockaddr*)&clnt_s._addr, &clnt_s._len);

	_pfd.push_back(pollfd());
	pollfd& pfd = _pfd.back();
	bzero(&pfd, sizeof (pfd));
	pfd.fd = clnt_s._fd;
	pfd.events = POLLIN | POLLERR | POLLHUP; // По умолчанию у клиентских сокетов запись открыта, POLLOUT не нужно смотреть
	pfd.revents = 0;

}

void 	Server::deleteClientSocket(vector<pollfd>::iterator& it)
{
	this->_clnt_sockets.erase(findSocketIter(it->fd));
	this->_pfd.erase(it);
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
