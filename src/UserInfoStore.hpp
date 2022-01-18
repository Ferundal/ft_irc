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
public:
	vector<User> _users_store;
	vector<User *> _connected_users;
	vector<Channel> _active_channels;
public:
	UserInfoStore();
	~UserInfoStore();
	User &CreateNewUser(string _nick, int _fd);
};


#endif //FT_IRC_USERINFOSTORE_HPP
