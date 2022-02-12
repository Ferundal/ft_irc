//
// Created by ferun on 02.02.2022.
//

#ifndef IRCSERC_BOT_HPP
#define IRCSERC_BOT_HPP

#include <string>
#include "IRC_Message.hpp"
#include "IRC_Connection.hpp"
#include "UserStore.hpp"
#include "reply_answers.hpp"
#define TO_STRING_100(X) #X
#define CODE_TO_STRING(X) TO_STRING_100(X)


class Bot {
private:
	std::string _bot_nick;
	IRC_Connection &_connection;
	IRC_Message _message;
	UserStore _user_store;
public:
	Bot(const char *bot_nick, const char *bot_password, IRC_Connection &connection);
	void	Login(const char *server_password);
	void	Start();
	class LoginExeption : public  std::exception {
		const char * what() const throw();
	};
	class UnknownMessageFormat : public  std::exception {
		const char * what() const throw();
	};
};


#endif //IRCSERC_BOT_HPP
