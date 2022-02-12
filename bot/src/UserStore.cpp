//
// Created by ferun on 10.02.2022.
//

#include "UserStore.hpp"

UserStore::UserStore(const char *bot_password, const std::string &bot_nick) : _bot_password(bot_password), _bot_nick(bot_nick) {}

int UserStore::AddNewUser(IRC_Message &message) {
	if (message._args.empty() || message._args[0] != _bot_nick)
		return (ERR_MESSAGE_NOT_FOR_ME);
	if (message._tail_message != _bot_password)
		return (ERR_WRONG_BOT_PASS);
	User *user_ptr = this->FindUserByNick(message._sender);
	if (user_ptr != NULL)
		return (0);
	_users.push_back(User(message._sender));
	return (0);
}

int UserStore::JoinChannel(IRC_Message &message) {
	User *user_ptr = this->FindUserByNick(message._sender);
	if (user_ptr == NULL)
		return (ERR_NOT_A_BOT_USER);
	if (user_ptr->JoinChannel(message._args[1]))
		return (ERR_ALREADY_ON_CHANNEL);
	return (0);
}

bool UserStore::IsTryGiveRights(IRC_Message &message) {
	User *user_ptr = this->FindUserByNick(message._sender);
	if (user_ptr == NULL)
		return (false);
	std::string *channel_name = user_ptr->FindChannelByName(message._args[0]);
	if (channel_name == NULL)
		return (false);
	return (true);
}

User *UserStore::FindUserByNick(std::string &nick) {
	std::vector<User>::iterator curr_user = _users.begin();
	std::vector<User>::iterator users_end = _users.end();
	while (curr_user != users_end) {
		if (curr_user->_nick == nick)
			return (&*curr_user);
		++curr_user;
	}
	return (NULL);
}

bool UserStore::ProceedKick(IRC_Message &message) {
	if (message._args.size() < 2)
		return (false);
	if (message._args[1] == _bot_nick) {
		std::vector<User>::iterator curr_user = _users.begin();
		std::vector<User>::iterator users_end = _users.end();
		std::vector<std::string>::iterator curr_user_channel;
		std::vector<std::string>::iterator user_channels_end;
		while (curr_user != users_end) {
			curr_user_channel = curr_user->_channel_names.begin();
			user_channels_end = curr_user->_channel_names.end();
			while (curr_user_channel != user_channels_end) {
				if (*curr_user_channel == message._args[0]) {
					curr_user->_channel_names.erase(curr_user_channel);
					return (true);
				}
				++curr_user_channel;
			}
			++curr_user;
		}
	}
	return (false);
}