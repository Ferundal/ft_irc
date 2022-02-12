//
// Created by ferun on 11.02.2022.
//

#ifndef FT_IRC_BOT_USER_HPP
#define FT_IRC_BOT_USER_HPP
#include "string"
#include "vector"

class User {
public:
	std::string _nick;
	std::vector<std::string> _channel_names;
	User(const std::string &nick);
	int JoinChannel(const std::string &channel_name);
	std::string *FindChannelByName(const std::string &channel_name);
};


#endif //FT_IRC_BOT_USER_HPP
