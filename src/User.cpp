//
// Created by ferun on 14.01.2022.
//

#include "User.hpp"

User::User(int _new_fd) : _fd(_new_fd), _is_active(false), _is_away(false) {}

const string &User::GetUserNick() const {
	return (this->_nick);
}

const int &User::GetUserFd() const {
	return (this->_fd);
}

const string &User::GetUserName() const {
	return(this->_user_name);
}

const string &User::GetUserHost() const {
	return (this->_host_name);
}

const string &User::GetUserRealName() const {
	return (this->_real_name);
}

int User::SetNick(const string &_new_nick) {
	if (_to_user_store->IsNickAvalable(_new_nick) == false) {
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

int User::SetActive() {
	if (this->_user_name.empty() == false && this->_nick.empty() == false) {
		this->_to_user_store->_connected_users.push_back(this);
		this->_is_active = true;
		return (0);
	}
	else
		return (1);
}

void User::SetAway(const string &_new_away_message) {
	this->_is_away = true;
	this->_away_message = _new_away_message;
}

void User::SetNotAway() {
	this->_is_away = false;
}

bool User::IsActive() const {
	return (this->_is_active);
}

bool User::IsAway() const {
	return (this->_is_away);
}

bool User::IsMemberOfChannel(Channel *_channel_ptr) {
	vector<Channel *>::iterator _curr_channel_ptr = this->_membership.begin();
	vector<Channel *>::iterator _channel_ptrs_end = this->_membership.end();
	while (_curr_channel_ptr != _channel_ptrs_end) {
		if (*_curr_channel_ptr == _channel_ptr)
			return (true);
		++_curr_channel_ptr;
	}
	return (false);
}

bool User::IsUserInfoSet() const {
	if (_user_name.empty())
		return false;
	return true;
}

int User::LeaveChannel(const string _channel_name) {
	vector<Channel *>::iterator _curr_membership = this->_membership.begin();
	vector<Channel *>::iterator _memberships_end = this->_membership.begin();
	while (_curr_membership != _memberships_end) {
		if ((*_curr_membership)->_channel_name == _channel_name) {

		}
		++_curr_membership;
	}
	return (1);
}


int User::JoinChannel(const string &_channel_name, const string &_channel_password) {
	Channel *_channel_ptr = _to_user_store->FindChannelByName(_channel_name);
	if (_channel_ptr == NULL) {
		this->_to_user_store->CreateNewChannel(this, _channel_name, _channel_password);
		return (0);
	}
	if (_channel_ptr->_invite_only_channel_flag == true &&
		_channel_ptr->IsInvited(this) == false)
		return (ERR_INVITEONLYCHAN);
	if (_channel_ptr->_password != _channel_password)
		return (ERR_BADCHANNELKEY);
	_channel_ptr->AddUser(*this);
	if (_channel_ptr->_invite_only_channel_flag == true)
		_channel_ptr->DeleteInvite(this);
	return (0);
}

int User::CancelChannelMembership(Channel *_cannel_to_leave) {
	vector<Channel *>::iterator _curr_membership_ptr = _membership.begin();
	vector<Channel *>::iterator _membership_end = _membership.end();
	while (_curr_membership_ptr != _membership_end) {
		if(*_curr_membership_ptr == _cannel_to_leave) {
			_membership.erase(_curr_membership_ptr);
			return (0);
		}
		++_curr_membership_ptr;
	}
	return (1);
}

string User::GetUserFullName() const {
	return (_nick + "!" + _user_name + "@" + _server_name);
}

UserInfoStore &User::ToStore() {
	return (*this->_to_user_store);
}

list<string> User::ChannelList(const string &_searching_channel_name) {
	list<string> _result;
	list<Channel>::iterator _curr_channel = this->ToStore()._active_channels.begin();
	list<Channel>::iterator _channels_end = this->ToStore()._active_channels.end();
	if (!_searching_channel_name.empty()) {
		while (_curr_channel != _channels_end) {
			if (_curr_channel->_channel_name == _searching_channel_name) {
				_channels_end = _curr_channel;
				++_channels_end;
				break;
			}
			++_curr_channel;
		}
	}
	while (_curr_channel != _channels_end) {
		if (this->IsMemberOfChannel(&*_curr_channel) ||
		((!_curr_channel->_secret_channel_flag) && (!_curr_channel->_private_channel_flag))) {
			_result.push_back(("" + _curr_channel->_channel_name) + (" " +
			_curr_channel->_user_store.size()) + " :" +
			_curr_channel->_channel_topic);
		} else {
			if (_curr_channel->_private_channel_flag) {
				_result.push_back("Prv" + _curr_channel->_channel_name);
			}
			++_curr_channel;
		}
	}
	return (_result);
}