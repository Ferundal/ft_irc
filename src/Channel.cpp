//
// Created by ferun on 14.01.2022.
//

#include "Channel.hpp"

Channel::Channel() {}

Channel::~Channel() {
	vector<User *>::iterator _begin = _user_store.begin();
	vector<User *>::iterator _curr = _user_store.end() ;
	while (_curr != _begin) {
		--_curr;
		(*_curr)->LeaveChannel(*this);
	}
}

void Channel::AddUser(User &_new_user) {
	_user_store.push_back(&_new_user);
	_new_user._membership.push_back(this);
}

