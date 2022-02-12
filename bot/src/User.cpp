//
// Created by ferun on 11.02.2022.
//

#include "User.hpp"

User::User(const std::string &nick) : _nick(nick) {}

int User::JoinChannel(const std::string &channel_name) {
	std::vector<std::string>::iterator curr_channel_name = _channel_names.begin();
	std::vector<std::string>::iterator channel_names_end = _channel_names.end();
	while (curr_channel_name != channel_names_end) {
		if (*curr_channel_name == channel_name)
			return (1);
		++curr_channel_name;
	}
	_channel_names.push_back(channel_name);
	return (0);
}

std::string *User::FindChannelByName(const std::string &channel_name) {
	std::vector<std::string>::iterator curr_channel_name = _channel_names.begin();
	std::vector<std::string>::iterator channel_names_end = _channel_names.end();
	while (curr_channel_name != channel_names_end) {
		if (*curr_channel_name == channel_name)
			return (&*curr_channel_name);
		++curr_channel_name;
	}
	return (NULL);
}