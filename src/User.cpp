//
// Created by ferun on 14.01.2022.
//

#include "User.hpp"

User::User(int _new_user_fd) : _fd(_new_user_fd) {}

void User::SetActive() {
	_user_store->_connected_users.push_back(this);
}

