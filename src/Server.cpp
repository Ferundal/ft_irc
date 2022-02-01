//
// Created by ferun on 05.01.2022.
//

#include "Server.hpp"

Server::Server(const char* port,const char* pass, int n_connect) try : _cnct_socket(), _pass(pass)
{
	int i = strtol(port, (char **)NULL, 10);
	if (errno == ERANGE || i == 0)
	{
		errno = 0;
		cout << "Port value error\n" << endl;
		throw exception();
	}
	this->_cnct_socket.binding(i);
	if (listen(_cnct_socket.getfd(), n_connect) == -1) throw exception();
	{
		_pfd.push_back(pollfd());
		bzero(&_pfd[0], sizeof (_pfd[0]));
		_pfd[0].fd = _cnct_socket.getfd();
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
	else{
		vector<pollfd>::iterator it = ++_pfd.begin();
		while(it < _pfd.end())
		{
			if(it->revents & POLLHUP)
			{
				//Пользователь отсоединился
				deleteClientSocket(it);
			}
			else if(it->revents & POLLIN )
			{
				//Пользователь выслал данныe
				try {
					readCommand(it);
					++it;
				}
				catch (Parser::UserDeleteException& e) // User delete
				{
					deleteClientSocket(it);
				}
			} else
				++it;
		}
	}
}

void	Server::readCommand(vector<pollfd>::iterator it)
{
	bool			r_frst_flag = false;
	int				r_len;
	char			r_buf[2] = {0,};
	ClientSocket&	sckt = *findSocketIter(it->fd);

	while (true)
	{
		r_len = recv(it->fd, &r_buf, 1, MSG_DONTWAIT);
		if (((r_len == 0) && (r_frst_flag == false)))
			throw Parser::UserDeleteException();
		else if (r_len == -1 || r_len == 0)
			break;
		else
			r_frst_flag = true;
		sckt._msg_buff.append(r_buf);
		if (sckt._msg_buff.find("\r\n") != string::npos)
			break;
	}
	if (sckt._msg_buff.find("\r\n") != string::npos)
	{
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
	new_pfd.events = POLLIN | POLLERR | POLLHUP;
	new_pfd.revents = 0;
	_pfd.push_back(new_pfd);
}

void 	Server::deleteClientSocket(vector<pollfd>::iterator& it)
{
	vector<ClientSocket>::iterator socket_it = findSocketIter(it->fd);

	string exit_message;
	vector<Channel *> receivers = socket_it->_usr_ptr->GetChannels();
	vector<Channel *>::iterator _curr_channel_ptr = receivers.begin();
	vector<Channel *>::iterator _channel_ptrs_end = receivers.end();
	while (_curr_channel_ptr != _channel_ptrs_end) {
		vector<User *>::const_iterator _curr_channel_user_ptr = (*_curr_channel_ptr)->GetChannelUsers().begin();
		vector<User *>::const_iterator _channel_user_ptrs_end = (*_curr_channel_ptr)->GetChannelUsers().end();
		while (_curr_channel_user_ptr != _channel_user_ptrs_end) {
			if (*_curr_channel_user_ptr != socket_it->_usr_ptr)
			{
				exit_message.clear();
				exit_message += ":" + socket_it->_usr_ptr->GetUserNick() + " PART " + (*_curr_channel_ptr)->GetChannelName() +"\r\n";
				send((*_curr_channel_user_ptr)->GetUserFd(),  socket_it->_msg_buff.data(), socket_it->_msg_buff.size(),0);
			}
			++_curr_channel_user_ptr;
		}
		++_curr_channel_ptr;
	}
	socket_it->_usr_ptr->ToStore().DeleteUser(socket_it->_usr_ptr);
	this->_clnt_sockets.erase(socket_it);
	close(it->fd);
	this->_pfd.erase(it);
}

vector<ClientSocket>::iterator Server::findSocketIter(int fd)
{
	for(vector<ClientSocket>::iterator it = _clnt_sockets.begin(); it < _clnt_sockets.end(); ++it)
	{
		if(it->_fd == fd)
			return (it);
	}
	throw Parser::UserDeleteException();
}

Server::~Server()
{}