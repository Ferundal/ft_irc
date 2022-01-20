//
// Created by ferun on 14.01.2022.
//

#include "Channel.hpp"

Channel::Channel() {}

// FIX

Channel::~Channel() {
	vector<User *>::iterator _begin = _user_store.begin();
	vector<User *>::iterator _curr_user_ptr = _user_store.end() ;
	while (_curr_user_ptr != _begin) {
		--_curr_user_ptr;
		(*_curr_user_ptr)->LeaveChannel(this->_channel_name);
	}
}

void Channel::AddUser(User &_new_user) {
	_user_store.push_back(&_new_user);
	_new_user._membership.push_back(this);
}

void Channel::DeleteUser(User &_new_user) {
	vector<User *>::iterator _begin = _user_store.begin();
	vector<User *>::iterator _curr_user_ptr = _user_store.end() - 1;
	while (_curr_user_ptr != _begin) {
		if(*_curr_user_ptr == &_new_user) {
			_user_store.erase(_curr_user_ptr);
			return;
		}
		--_curr_user_ptr;
	}
}

