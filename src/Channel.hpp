//
// Created by ferun on 14.01.2022.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP
#include <vector>
#include <string>
#include "User.hpp"

using std::vector;
using std::string;

class User;

class Channel {
private:
	string _channel_name;
	string _channel_topic;
	vector<User>::iterator _owner;
	vector<User *> _user_store;
public:
	Channel();
	~Channel();
	void AddUser(User &_new_user);
};


#endif //FT_IRC_CHANNEL_HPP
