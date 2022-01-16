//
// Created by ferun on 05.01.2022.
//


#include "Server.hpp"

Server::Server(int n_connect)try : _cnct_socket()
{
	listen(_cnct_socket.getfd(), n_connect); //  n_connect - максимальная длина, до которой может расти очередь ожидающих соединений
	// Внесение данных первого элемента ConnetSocket в pfd
	{
		_pfd.push_back(pollfd());
		_pfd[0].fd = _cnct_socket.getfd();
		/*
		 *  POLLIN -	Можно считывать данные
		 *  POLLHUP -	Положили трубку
		 *  POLLERR -	Ошибка
		 */
		_pfd[0].events = POLLIN | POLLERR;

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
	poll(_pfd.data(), _pfd.size(), -1);
	if ((_pfd[0].revents & POLLIN) == POLLIN)
	{
		// Новый пользователь
		_clnt_sockets.push_back(ClientSocket());
		_clnt_sockets.back()._fd = accept(_cnct_socket.getfd(), (sockaddr*)&_clnt_sockets.back()._addr, &_clnt_sockets.back()._len);
		_pfd.push_back(pollfd());
		_pfd.back().fd = _clnt_sockets.back()._fd;
		_pfd.back().events = POLLIN | POLLERR;
		poll(_pfd.data(), _pfd.size(), -1);
	}
	for(vector<pollfd>::iterator it = _pfd.begin(); it < _pfd.end(); ++it)
	{
		if((it->revents & POLLIN) == POLLIN)
		{
			//Пользователь на сокете ждет ответ
			int r_len;

			r_len = read( it->fd, _msg_buf, 1000 );
			this->_parser.stringParser((char *)_msg_buf);
			write( 1, _msg_buf, r_len );
			// write( it->fd, "OK", 2 );
		}
	}
}

Server::~Server() {}