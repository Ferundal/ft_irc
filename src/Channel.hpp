//
// Created by ferun on 14.01.2022.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP
#include <vector>
#include <string>
#include "User.hpp"
#include "UserInfoStore.hpp"

using std::vector;
using std::string;

class User;
class UserInfoStore;

class Channel {
	friend class User;
	friend class UserInfoStore;
private:
	string _channel_name;
	string _channel_topic;
	vector<User>::iterator _owner;
	vector<User *> _user_store;
private:
	void AddUser(User &_new_user);
	void DeleteUser(User &_new_user);
public:
	Channel();
	~Channel();
};


#endif //FT_IRC_CHANNEL_HPP
