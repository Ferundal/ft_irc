//
// Created by ferun on 14.01.2022.
//

#ifndef FT_IRC_USERINFOSTORE_HPP
#define FT_IRC_USERINFOSTORE_HPP
#include <list>
#include <iostream>
#include "User.hpp"
#include "Channel.hpp"


using std::list;
using std::string;

class User;
class Channel;

class UserInfoStore {
	friend class User;
	friend class Channel;
private:
	list<User> _users_store;
	list<User *> _connected_users;
	list<Channel> _active_channels;
public:
	UserInfoStore();
	~UserInfoStore();
	bool IsNickAvalable(const string _searching_nick) const;
	bool IsUserActive(const string _searching_nick) const;
	User &CreateNewUser(int _new_user_fd);
	User *FindUserByNick(string _searching_nick);
	Channel *FindChannelByName(string _searching_channel_name);
	void PrintUserInfoStore();
};


#endif //FT_IRC_USERINFOSTORE_HPP
