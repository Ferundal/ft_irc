//
// Created by ferun on 14.01.2022.
//

#include "User.hpp"
#include <stdlib.h>

User::User(int _new_fd) : _fd(_new_fd), _is_activated(false), _is_away(false) {}

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

const vector<Channel *> &User::GetChannels() const {
	return (this->_membership);
}

int User::SetNick(const string &_new_nick) {
	if (_new_nick == this->_nick)
		return (0);
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

int User::SetActivated() {
	if (this->_user_name.empty() == false && this->_nick.empty() == false) {
		this->_to_user_store->_connected_users.push_back(this);
		this->_is_activated = true;
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

bool User::IsActivated() const {
	return (this->_is_activated);
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

int User::LeaveChannel(const string &_channel_name) {
	if (this->ToStore().FindChannelByName(_channel_name) == NULL) {
		return (ERR_NOSUCHCHANNEL);
	}
	return (this->ToStore().LeaveChannel(this, _channel_name));
}


int User::JoinChannel(const string &_channel_name, const string &_channel_password) {
	Channel *_channel_ptr = _to_user_store->FindChannelByName(_channel_name);
	if (_channel_ptr == NULL) {
		this->_to_user_store->CreateNewChannel(this, _channel_name, _channel_password);
		return (0);
	}
	if (this->IsMemberOfChannel(_channel_ptr))
		return (ERR_USERONCHANNEL);
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

const string &User::GetAwayMessage() const {
	return (_away_message);
}

UserInfoStore &User::ToStore() {
	return (*this->_to_user_store);
}

string ft_to_string(int num) {
	string _result;
	char curr_char;
	if (num == 0)
		return (string("0"));
	while (num / 10 != 0) {
		curr_char = num % 10 + '0';
		_result = curr_char + _result;
		num = num / 10;
	}
	_result = (char)(num + '0') + _result;
	return (_result);
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
			_result.push_back(_curr_channel->_channel_name + " " +
									ft_to_string(_curr_channel->_user_store.size()) + " :" +
			_curr_channel->_channel_topic);
		} else {
			if (_curr_channel->_private_channel_flag) {
				_result.push_back("Prv" + _curr_channel->_channel_name);
			}
		}
		++_curr_channel;
	}
	return (_result);
}

int User::SendInvite(const string &_invited_user_nick,
					 const string &_channel_invite_to) {
	User *_invited_user_ptr = this->ToStore().FindUserByNick(_invited_user_nick);
	if (_invited_user_ptr == NULL)
		return (ERR_NOSUCHNICK);
	vector<Channel *>::iterator _memberships_begin = this->_membership.begin();
	vector<Channel *>::iterator _curr_membership = this->_membership.end();
	while (_curr_membership != _memberships_begin) {
		--_curr_membership;
		if ((*_curr_membership)->_channel_name == _channel_invite_to) {
			if (!_invited_user_ptr->IsMemberOfChannel(*_curr_membership)) {
				return (ERR_USERONCHANNEL);
			}
			if ((*_curr_membership)->_invite_only_channel_flag == true) {
				if (!(*_curr_membership)->IsOperator(this)) {
					return (ERR_CHANOPRIVSNEEDED);
				}
				(*_curr_membership)->AddInvite(_invited_user_ptr);
			}
			return (0);
		}
	}
	return (ERR_NOTONCHANNEL);
}

int User::ChangeTopic(const string &_channel_to_change_topic,
					  const string &_new_topic) {
	vector<Channel *>::iterator _memberships_begin = this->_membership.begin();
	vector<Channel *>::iterator _curr_membership = this->_membership.end();
	while (_curr_membership != _memberships_begin) {
		--_curr_membership;
		if ((*_curr_membership)->_channel_name == _channel_to_change_topic) {
			if ((*_curr_membership)->_topic_for_operators_flag == true &&
				!(*_curr_membership)->IsOperator(this)) {
				return (ERR_CHANOPRIVSNEEDED);
			}
			(*_curr_membership)->_channel_topic = _new_topic;
			(*_curr_membership)->_is_topic_set = true;
			return (0);
		}
	}
	return (ERR_NOTONCHANNEL);
}

int User::GetTopic(const string &_channel_to_get_topic, string &_topic_store) {
	vector<Channel *>::iterator _memberships_begin = this->_membership.begin();
	vector<Channel *>::iterator _curr_membership = this->_membership.end();
	while (_curr_membership != _memberships_begin) {
		--_curr_membership;
		if ((*_curr_membership)->_channel_name == _channel_to_get_topic) {
			if (!(*_curr_membership)->IsTopicSet())
				return (RPL_NOTOPIC);
			_topic_store = (*_curr_membership)->_channel_topic;
			return (0);
		}
	}
	return (ERR_NOTONCHANNEL);
}