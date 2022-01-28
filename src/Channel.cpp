//
// Created by ferun on 14.01.2022.
//

#include "Channel.hpp"

Channel::Channel(User *_owner_ptr, const string &_new_channel_name, const string&_new_channel_password) :
						_channel_name(_new_channel_name),
						_password(_new_channel_password),
						_private_channel_flag(false),
						_secret_channel_flag(false),
						_invite_only_channel_flag(false),
						_topic_for_operators_flag(false),
						_no_messages_from_outside_channel_flag(false),
						_moderated_channel_flag(false),
						_limited_users_on_channel(-1),
						_owner(_owner_ptr)
						{
	_user_store.push_back(_owner_ptr);
	_operators.push_back(_owner_ptr);
}

// FIX

Channel::~Channel() {
	vector<User *>::iterator _begin = _user_store.begin();
	vector<User *>::iterator _curr_user_ptr = _user_store.end() ;
	while (_curr_user_ptr != _begin) {
		--_curr_user_ptr;
		(*_curr_user_ptr)->CancelChannelMembership(this);
	}
}

void Channel::AddUser(User &_new_user) {
	_user_store.push_back(&_new_user);
	_new_user._membership.push_back(this);
}

int Channel::DeleteUser(User *_user_to_delete) {
	vector<User *>::iterator _begin = _user_store.begin();
	vector<User *>::iterator _curr_user_ptr = _user_store.end();
	while (_curr_user_ptr != _begin) {
		--_curr_user_ptr;
		if(*_curr_user_ptr == _user_to_delete) {
			this->_user_store.erase(_curr_user_ptr);
			this->DeleteFromOperators(_user_to_delete);
			return (0);
		}
	}
	return (1);
}

void Channel::DeleteFromOperators(User *_user_to_delete) {
	vector<User *>::iterator _curr_moderator = _operators.begin();
	vector<User *>::iterator _moderators_end = _operators.end();
	while (_curr_moderator != _moderators_end) {
		if (*_curr_moderator == _user_to_delete) {
			_operators.erase(_curr_moderator);
			if (_operators.empty()) {
				if (!_user_store.empty()) {
					_operators.push_back(*_user_store.begin());
					if (_user_to_delete == _owner) {
						_owner = *_operators.begin();
					}
 				}
			}
			return;
		}
		++_curr_moderator;
	}

}

void Channel::AddInvite(User *_new_invite_user_ptr) {
	vector<User *>::iterator _curr_invited_user = this->_invites.begin();
	vector<User *>::iterator _invited_users_end = this->_invites.end();
	while (_curr_invited_user != _invited_users_end) {
		if (*_curr_invited_user == _new_invite_user_ptr)
			return;
		++_curr_invited_user;
	}
	this->_invites.push_back(_new_invite_user_ptr);
}

bool Channel::IsInvited(User *_checked_user_ptr) {
	vector<User *>::iterator _curr_invited_user = this->_invites.begin();
	vector<User *>::iterator _invited_users_end = this->_invites.end();
	while (_curr_invited_user != _invited_users_end) {
		if (*_curr_invited_user == _checked_user_ptr) {
			return (true);
		}
		++_curr_invited_user;
	}
	return (false);
}

void Channel::DeleteInvite(User *_user_for_invitation_delete) {
	vector<User *>::iterator _curr_invited_user = this->_invites.begin();
	vector<User *>::iterator _invited_users_end = this->_invites.end();
	while (_curr_invited_user != _invited_users_end) {
		if (*_curr_invited_user == _user_for_invitation_delete) {
			this->_invites.erase(_curr_invited_user);
			return;
		}
		++_curr_invited_user;
	}
}
