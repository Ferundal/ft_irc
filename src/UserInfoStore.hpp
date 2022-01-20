//
// Created by ferun on 14.01.2022.
//

#ifndef FT_IRC_USERINFOSTORE_HPP
#define FT_IRC_USERINFOSTORE_HPP
#include <vector>
#include "User.hpp"
#include "Channel.hpp"

using std::vector;
using std::string;

class User;
class Channel;

class UserInfoStore {
	friend class User;
	friend class Channel;
private:
	vector<User> _users_store;
	vector<User *> _connected_users;
	vector<Channel> _active_channels;
private:

public:
	UserInfoStore();
	~UserInfoStore();
	User &CreateNewUser();
	User *FindUserByNick(string _searching_nick);
	Channel *FindChannelByName(string _searching_channel_name);
};


#endif //FT_IRC_USERINFOSTORE_HPP