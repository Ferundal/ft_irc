//
// Created by Radicchio Sarah on 11.01.2022.
//


#include "ClientSocket.hpp"

ClientSocket::~ClientSocket()
{
	close(_fd);
}