//
// Created by ferun on 05.01.2022.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "Parser.hpp"
#include "ConnectSocket.hpp"
#include "ClientSocket.hpp"
#include "Channel.hpp"
#include "exception"
#include <sys/types.h>
#include <sys/socket.h>
#include "vector"
#include <sys/poll.h>
#include "errno.h"
#include <unistd.h>
#include <fcntl.h>
#include "UserInfoStore.hpp"

#define MAX_CONNECT_CROWD 128

using namespace std;

class Server {
public:
	explicit Server(const char* port,const char* pass, int n_connect=MAX_CONNECT_CROWD);
	void 	grabConnection();

	~Server();
private:
	void 	deleteClientSocket(vector<pollfd>::iterator& it);
	void	readCommand(vector<pollfd>::iterator it);
	void	addNewClientSocket();
	vector<ClientSocket>::iterator findSocketIter(int fd);


	ConnectSocket			_cnct_socket;
	vector<ClientSocket>	_clnt_sockets;
	const char*				_pass;;
	vector<pollfd>			_pfd;
	UserInfoStore			_user_bd;
	Parser					_parser;
};

#endif //FT_IRC_SERVER_HPP
