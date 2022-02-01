//
// Created by ferun on 14.01.2022.
//

#include "UserInfoStore.hpp"

UserInfoStore::UserInfoStore() {}

UserInfoStore::~UserInfoStore() {}

User &UserInfoStore::CreateNewUser(int _new_user_fd) {
	_users_store.push_back(User(_new_user_fd));
	User& usr = _users_store.back();
	usr._to_user_store = this;
	return (usr);
}

void UserInfoStore::DeleteUser(User *_user_to_delete) {
	if (_user_to_delete->_is_activated) {
		{
			list<User *>::iterator _curr_active_user = _connected_users.begin();
			list<User *>::iterator _active_user_end = _connected_users.end();
			while (_curr_active_user != _active_user_end) {
				if (*_curr_active_user == _user_to_delete) {
					_connected_users.erase(_curr_active_user);
					break;
				}
				++_curr_active_user;
			}
		}
		vector<Channel *>::iterator _joined_channels_begin = _user_to_delete->_membership.begin();
		vector<Channel *>::iterator _curr_joined_channel = _user_to_delete->_membership.end();
		while (_curr_joined_channel != _joined_channels_begin)
		{
			--_curr_joined_channel;
			(*_curr_joined_channel)->DeleteUser(_user_to_delete);
			if ((*_curr_joined_channel)->_user_store.empty()) {
				DeleteChannel((*_curr_joined_channel)->_channel_name);
			}
			_user_to_delete->_membership.erase(_curr_joined_channel);
		}
	}
	list<User>::iterator _curr_user = _users_store.end();
	list<User>::iterator _users_store_begin = _users_store.begin();
	while (_curr_user != _users_store_begin) {
		--_curr_user;
		if (&*_curr_user == _user_to_delete) {
			_users_store.erase(_curr_user);
			break;
		}
	}
}

void UserInfoStore::DeleteChannel(const string &_delete_channel_name) {
	list<Channel>::iterator _curr_channel = _active_channels.begin();
	list<Channel>::iterator _active_channels_end = _active_channels.end();
	while (_curr_channel !=  _active_channels_end) {
		if (_curr_channel->_channel_name == _delete_channel_name) {
			_active_channels.erase(_curr_channel);
			return;
		}
		++_curr_channel;
	}
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

int UserInfoStore::CreateNewChannel(User *_owner_ptr, const string &_new_channel_name,
									const string &_new_channel_password) {
	_active_channels.push_back(Channel(_owner_ptr, _new_channel_name, _new_channel_password));
	_owner_ptr->_membership.push_back(&_active_channels.back());
	return (0);
}

int UserInfoStore::LeaveChannel(User *_member_user_ptr,
								const string &_channel_to_leave) {
	vector<Channel *>::iterator _memberships_begin = _member_user_ptr->_membership.begin();
	vector<Channel *>::iterator _curr_membership = _member_user_ptr->_membership.end();
	while (_curr_membership != _memberships_begin) {
		--_curr_membership;
		if ((*_curr_membership)->_channel_name == _channel_to_leave) {
			(*_curr_membership)->DeleteUser(_member_user_ptr);
			if ((*_curr_membership)->_user_store.empty()) {
				this->DeleteChannel(_channel_to_leave);
			}
			_member_user_ptr->_membership.erase(_curr_membership);
			(*_curr_membership)->TakeAwayVoiceRights(_member_user_ptr);
			return (0);
		}
	}
	return (ERR_NOTONCHANNEL);
}

const list<User *> &UserInfoStore::GetAllActiveUsers() const {
	return (_connected_users);
}

void UserInfoStore::PrintUserInfoStore() {
	std::cout << "-------------All users------------" << std::endl;
	{
		list<User>::iterator _curr_user = _users_store.begin();
		list<User>::iterator _end_user = _users_store.end();
		while (_curr_user != _end_user) {
			std::cout << _curr_user->_fd << " " << _curr_user->_nick << " "
					  << _curr_user->_real_name << std::endl;
			++_curr_user;
		}
	}
	std::cout << "-------------All active users------------" << std::endl;
	{
		list<User *>::iterator _curr_nick = _connected_users.begin();
		list<User *>::iterator _end_nick = _connected_users.end();
		while (_curr_nick != _end_nick) {
			std::cout << *_curr_nick << std::endl;
			++_curr_nick;
		}
	}
	std::cout << "-------------Channels------------" << std::endl;
	{
		list<Channel>::iterator _curr_channel_ptr = _active_channels.begin();
		list<Channel>::iterator _channels_ptr_end = _active_channels.end();
		while (_curr_channel_ptr != _channels_ptr_end) {
			std::cout << _curr_channel_ptr->_channel_name << std::endl;
			++_curr_channel_ptr;
		}
	}
	std::cout << "-------------End------------" << std::endl << std::endl;
}