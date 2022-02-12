//
// Created by ferun on 10.02.2022.
//

#ifndef FT_IRC_BOT_USERSTORE_HPP
#define FT_IRC_BOT_USERSTORE_HPP
#include "IRC_Message.hpp"
#include "vector"
#include "User.hpp"
#define ERR_WRONG_BOT_PASS 1
#define ERR_NOT_A_BOT_USER 2
#define ERR_ALREADY_ON_CHANNEL 3
#define ERR_MESSAGE_NOT_FOR_ME 4


class UserStore {
private:
	std::string _bot_password;
	const std::string &_bot_nick;
	std::vector<User> _users;
	UserStore();
public:
	UserStore(const char *bot_password, const std::string &bot_nick);
	int AddNewUser(IRC_Message &message);
	int JoinChannel(IRC_Message &message);
	bool IsTryGiveRights(IRC_Message &message);
	User *FindUserByNick(std::string &nick);
	bool ProceedKick(IRC_Message &message);
};


#endif //FT_IRC_BOT_USERSTORE_HPP
