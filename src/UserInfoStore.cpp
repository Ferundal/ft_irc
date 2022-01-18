//
// Created by ferun on 14.01.2022.
//

#include "UserInfoStore.hpp"

UserInfoStore::UserInfoStore() {}

UserInfoStore::~UserInfoStore() {}

User &UserInfoStore::CreateNewUser(string _nick, int _fd) {
	_users_store.push_back(User(_fd));
	vector<User>::iterator _curr = --_users_store.end();
	_curr->_user_store = *this;
	return (*_curr);
}