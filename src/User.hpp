//
// Created by ferun on 14.01.2022.
//

#ifndef FT_IRC_USER_HPP
#define FT_IRC_USER_HPP
#include <string>
#include <vector>
#include <exception>
#include "Channel.hpp"
#include "UserInfoStore.hpp"

using std::string;
using std::vector;

class Channel;

class User {
	friend class Channel;
	friend class UserInfoStore;
private:
	int					_fd;
	string				_real_name;
	string				_nick;
	UserInfoStore		*_user_store;
	vector<Channel *>	_membership;
private:
	User(int _new_user_fd);
public:
	int SetNick(string _new_nick);
	int SetUserInfo(string _mew_real_name);
	int SetActive(void)n;
	int JoinChannel(string _channel_name);
	int LeaveChannel(string _channel_name);
};


#endif //FT_IRC_USER_HPP
