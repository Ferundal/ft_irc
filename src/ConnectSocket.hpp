#ifndef IRCSERC_CONNECTSOCKET_HPP
#define IRCSERC_CONNECTSOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "string"
#include <cstring>
#include "exception"

#define PORT	30001

using namespace std;


class ConnectSocket
{
public:
	class SocketConfigException : public std::exception
	{
	public:
		SocketConfigException(const string str) : _str(str){};
		virtual ~SocketConfigException() throw() {};
		const char * what() const throw(){return _str.data();};
	private:
		string _str;
	};
public:

	/*
	 * protocol=0 - по умолчанию - в соответствии с domain
	 */
	explicit	ConnectSocket(int domain=AF_INET, int type=SOCK_STREAM, int protocol=0) throw (SocketConfigException);
	int			getfd() const;
	void		binding(int port=PORT, int family=AF_INET, in_addr_t ip=INADDR_ANY);

	~ConnectSocket();
private:
	int			_fd;
	sockaddr_in	_addr;
};


#endif //IRCSERC_CONNECTSOCKET_HPP
