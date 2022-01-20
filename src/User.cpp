//
// Created by ferun on 14.01.2022.
//

#include "User.hpp"

User::User() : _is_active(false) {}

int User::SetNick(const string &_new_nick) {
	if (_user_store->IsNickAvalable(_new_nick) == false) {
		return (1);
	}
	this->_nick = _new_nick;
	return (0);
}

int User::SetUserInfo(const string &_new_user_name, const string &_new_host_name,
					  const string &_new_server_name, const string &_new_real_name) {
	if (this->_user_name.empty() == false)
		return (1);
	else {
		this->_user_name = _new_user_name;
		this->_host_name = _new_host_name;
		this->_server_name = _new_server_name;
		this->_real_name = _new_real_name;
		return (0);
	}
}

bool User::IsActive() const {
	return (_is_active);
}

int User::SetActive() {
	if (this->_user_name.empty() == false && this->_nick.empty() == false) {
		_user_store->_connected_users.push_back(this);
		_is_active = true;
		return (0);
	}
	else
		return (1);
}

int User::JoinChannel(const string &_channel_name) {
	Channel *_channel_ptr = _user_store->FindChannelByName(_channel_name);
	if (_channel_ptr == NULL)
		return (1);
	_channel_ptr->AddUser(*this);
	return (0);
}

int User::LeaveChannel(const string &_channel_name) {
	vector<Channel *>::iterator _curr_channel_ptr = _membership.begin();
	vector<Channel *>::iterator _end = _membership.end();
	while (_curr_channel_ptr != _end) {
		if((*_curr_channel_ptr)->_channel_name == _channel_name) {
			(*_curr_channel_ptr)->DeleteUser(*this);
			return (0);
		}
		++_curr_channel_ptr;
	}
	return (1);
}

string User::GetUserFullName() const {
	return (_nick);
}

UserInfoStore &User::ToStore() {
	return (*this->_user_store);
}

