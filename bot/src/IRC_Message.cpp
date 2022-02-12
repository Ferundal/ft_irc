//
// Created by ferun on 04.02.2022.
//

#include "IRC_Message.hpp"

IRC_Message::IRC_Message() : _is_correct(false) {}

void IRC_Message::Parcer(std::string message) {
	_is_correct = false;
	if (message.size() < 5 || message.size() > 510)
		return;
	size_t position_begin = message.find(':');
	if (position_begin != 0)
		return;
	++position_begin;
	size_t position_end = message.find(' ', position_begin);
	if (position_end == std::string::npos)
		return;
	_sender = std::string(message, position_begin, position_end - position_begin);
	if (_sender.find('!') != std::string::npos) {
		_sender.erase(_sender.find('!'));
	}
	position_begin = position_end + 1;
	position_end = message.find(' ', position_begin);
	if (position_end == std::string::npos)
		return;
	_command = std::string(message, position_begin, position_end - position_begin);
	position_begin = position_end + 1;
	position_end = message.find(' ', position_begin);
	_args.clear();
	while (message[position_begin] != ':' && position_end != std::string::npos) {
		_args.push_back(std::string(message, position_begin, position_end - position_begin));
		position_begin = position_end + 1;
		position_end = message.find(' ', position_begin);
	}
	if (message[position_begin] == ':') {
		_tail_message = std::string(message, position_begin + 1, message.size() - position_begin);
		_is_correct = true;
		return;
	}
	_args.push_back(std::string(message, position_begin, message.size() - position_begin));
	_tail_message.clear();
	_is_correct = true;
}

std::ostream &operator <<(std::ostream &outstream, const IRC_Message &origin) {
	outstream << ":" << origin._sender << " " << origin._command;
	for (size_t i = 0; i < origin._args.size(); i++)
		outstream << " " << origin._args[i];
	outstream << " :" << origin._tail_message;
	return (outstream);
}