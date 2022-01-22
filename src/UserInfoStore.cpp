//
// Created by ferun on 14.01.2022.
//

#include "UserInfoStore.hpp"

UserInfoStore::UserInfoStore() {}

UserInfoStore::~UserInfoStore() {}

User &UserInfoStore::CreateNewUser(int _new_user_fd) {
	_users_store.push_back(User(_new_user_fd));
	User& usr = _users_store.back();
	usr._user_store = this;
	return (usr);
}

bool UserInfoStore::IsNickAvalable(string const _searching_nick) const {
	list<User>::const_iterator _curr_user = _users_store.begin();
	list<User>::const_iterator _end = _users_store.end();
	while (_curr_user != _end) {
		if (_curr_user->_nick == _searching_nick)
			return (false);
		++_curr_user;
	}
	return (true);
}

bool UserInfoStore::IsUserActive(string const _searching_nick) const {
	list<User *>::const_iterator _curr_active_user_name = _connected_users.begin();
	list<User *>::const_iterator _end = _connected_users.end();
	while (_curr_active_user_name != _end) {
		if ((*_curr_active_user_name)->_nick == _searching_nick)
			return (true);
		++_curr_active_user_name;
	}
	return (false);
}

User *UserInfoStore::FindUserByNick(string _searching_nick) {
	list<User *>::iterator _curr_user = _connected_users.begin();
	list<User *>::iterator _end = _connected_users.end();
	while (_curr_user != _end) {
		if ((*_curr_user)->_nick == _searching_nick)
			return (*_curr_user);
		++_curr_user;
	}
	return (NULL);
}

Channel *UserInfoStore::FindChannelByName(string _searching_channel_name) {
	list<Channel>::iterator _curr_channel = _active_channels.begin();
	list<Channel>::iterator _end = _active_channels.end();
	while (_curr_channel != _end) {
		if(_curr_channel->_channel_name == _searching_channel_name)
			return (&*_curr_channel);
		++_curr_channel;
	}
	return (NULL);
}

void UserInfoStore::PrintUserInfoStore() {
	list<User>::iterator _curr_user = _users_store.begin();
	list<User>::iterator _end_user = _users_store.end();
	while (_curr_user != _end_user) {
		std::cout << _curr_user->_fd << " " << _curr_user->_nick << " " << _curr_user->_real_name << std::endl;
		++_curr_user;
	}
	list<User *>::iterator _curr_nick = _connected_users.begin();
	list<User *>::iterator _end_nick = _connected_users.end();
	while (_curr_nick != _end_nick) {
		std::cout << *_curr_nick << std::endl;
		++_curr_nick;
	}

}