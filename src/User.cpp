//
// Created by ferun on 14.01.2022.
//

#include "User.hpp"

User::User() {}

int User::SetNick(string _new_nick) {
	if (_user_store->FindUserByNick(_new_nick) != NULL) {
		return (1);
	}
	this->_nick = _new_nick;
	return (0);
}

int User::SetUserInfo(string _mew_real_name) {
	this->_real_name = _mew_real_name;
}

int User::SetActive() {
	_user_store->_connected_users.push_back(this);
}

int User::JoinChannel(string _channel_name) {
	Channel *_channel_ptr = _user_store->FindChannelByName(_channel_name);
	if (_channel_ptr == NULL)
		return (1);
	_channel_ptr->AddUser(*this);
	return (0);
}

int User::LeaveChannel(string _channel_name) {
	vector<Channel *>::iterator _curr_channel_ptr = _membership.begin();
	vector<Channel *>::iterator _end = _membership.end();
	while (_curr_channel_ptr != _end) {
		if((*_curr_channel_ptr)->_channel_name == _channel_name) {

		}
		++_curr_channel;
	}
}


