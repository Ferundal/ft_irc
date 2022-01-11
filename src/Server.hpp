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

#define CNNCT_MAX 1050

using namespace std;

class Server {
public:
	explicit Server(int n_connect=5);
	void listening();
	void communication();

	~Server();
private:
	ConnectSocket _cnct_socket;
	vector<ClientSocket> _clnt_sockets;
	vector<pollfd> _pfd;
	char* _msg_buf[1000];
};


#endif //FT_IRC_SERVER_HPP
