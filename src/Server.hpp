//
// Created by ferun on 05.01.2022.
//

#ifndef FT_IRC_SERVER_HPP
#define FT_IRC_SERVER_HPP

#include "ConnectSocket.hpp"
#include "ClientSocket.hpp"
#include "exception"
#include <sys/types.h>
#include <sys/socket.h>
#include "vector"
#include "Parser.hpp"

#include "fstream"
#include "iostream"
#include "algorithm"
#include "errno.h"

#define MAX_CONNECT_CROWD 10

using namespace std;

class Server {
public:
	explicit Server(int n_connect=MAX_CONNECT_CROWD);
	void listening();
	void communication();

	~Server();
private:
	bool chekMsgEnding(string& str);
	vector<ClientSocket>::iterator findSocketIter(int fd);


	ConnectSocket _cnct_socket;
	vector<ClientSocket> _clnt_sockets;
	vector<pollfd> _pfd;
	Parser _parser;
};

#endif //FT_IRC_SERVER_HPP
