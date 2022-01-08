//
// Created by Radicchio Sarah on 08.01.2022.
//

#include "Socket.hpp"

Socket::Socket(int domain, int type, int protocol) throw (exception)
{
	if ((this->ret_status = socket(domain, type, protocol)) == -1) throw exception();
}


//char* Socket::getsockopt()
//{
//	int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
//}
