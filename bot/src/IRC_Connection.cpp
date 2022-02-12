//
// Created by ferun on 04.02.2022.
//

#include "IRC_Connection.hpp"


IRC_Connection::IRC_Connection() : _read_amount(0), _message_end(std::string::npos) {
	bzero (&this->_socket_info, sizeof (this->_socket_info));
	bzero (&this->_pfd, sizeof (this->_pfd));
}

void IRC_Connection::EstablishConnection(const char *server_host,
									const char *server_port) {
	_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (_socket_fd == -1)
		throw SoketExeption();
	if (inet_aton(server_host, &this->_socket_info.sin_addr) == 0)
		throw ConnectExeption();
	this->_socket_info.sin_family = AF_INET;
	this->_socket_info.sin_port =  htons(atoi(server_port));
	if(connect(_socket_fd, (struct sockaddr*)&this->_socket_info, sizeof(this->_socket_info)))
		throw ConnectExeption();
	_pfd.fd = _socket_fd;
	_pfd.events = POLLIN | POLLERR | POLLHUP;
	_pfd.revents = 0;
}

void IRC_Connection::SendIRC_Command(std::string &command) {
	std::cout << "Output->" << command << std::endl;
	std::string message = command + "\r\n";
	send(_socket_fd, message.data(), message.size(), 0);
}

void IRC_Connection::GetMessage(IRC_Message &message_to_fill) {
	while (_message_end == std::string::npos) {
		poll(&_pfd, 1, -1);
		_read_amount += recv(_socket_fd, _buffer, MAX_IRC_MESSAGE_LEN, 0);
		_command_string += std::string(_buffer, _read_amount);
		_read_amount = 0;
		_message_end = _command_string.find("\r\n", 0);
	}
	if (_command_string.size() < MAX_IRC_MESSAGE_LEN) {
		message_to_fill.Parcer(std::string(_command_string, 0, _message_end));
		std::cout << "Input ->" << message_to_fill << std::endl;
		_command_string.erase(0, _message_end + 2);
		_message_end = _command_string.find("\r\n", 0);
		return;
	} else {
		_command_string.erase(0, _message_end + 2);
		_message_end = _command_string.find("\r\n", 0);
	}
}

const char *IRC_Connection::SoketExeption::what() const throw() {
	return ("Can't create socket for IRC connection!\n");
}

const char *IRC_Connection::ConnectExeption::what() const throw() {
	return ("Can't establish connection with such host and port!\n");
}
