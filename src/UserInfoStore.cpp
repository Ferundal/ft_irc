//
// Created by ferun on 14.01.2022.
//

#include "UserInfoStore.hpp"

UserInfoStore::UserInfoStore() {}

UserInfoStore::~UserInfoStore() {}

User &UserInfoStore::CreateNewUser() {
	_users_store.push_back(User());
	vector<User>::iterator _curr = --_users_store.end();
	_curr->_user_store = this;
	return (*_curr);
}

User *UserInfoStore::FindUserByNick(string _searching_nick) {
	vector<User *>::iterator _curr_active_user_ptr = _connected_users.begin();
	vector<User *>::iterator _end = _connected_users.end();
	while (_curr_active_user_ptr != _end) {
		if ((*_curr_active_user_ptr)->_nick == _searching_nick)
			return (*_curr_active_user_ptr);
		++_curr_active_user_ptr;
	}
	return (NULL);
}

Channel *UserInfoStore::FindChannelByName(string _searching_channel_name) {
	vector<Channel>::iterator _curr_channel = _active_channels.begin();
	vector<Channel>::iterator _end = _active_channels.end();
	while (_curr_channel != _end) {
		if(_curr_channel->_channel_name == _searching_channel_name)
			return (&*_curr_channel);
		++_curr_channel;
	}
	return (NULL);
}