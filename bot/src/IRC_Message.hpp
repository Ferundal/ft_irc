//
// Created by ferun on 04.02.2022.
//

#ifndef FT_IRC_BOT_COMMAND_HPP
#define FT_IRC_BOT_COMMAND_HPP
#include <vector>
#include <string>
#include <iostream>


class IRC_Message {
public:
	bool						_is_correct;
	std::string					_sender;
	std::string					_command;
	std::vector<std::string>	_args;
	std::string					_tail_message;
public:
	IRC_Message();
	void Parcer(std::string message);
};

std::ostream &operator <<(std::ostream &outstream, const IRC_Message &origin);
#endif //FT_IRC_BOT_COMMAND_HPP
