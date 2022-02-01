//
// Created by ferun on 14.01.2022.
//

#include "Channel.hpp"

Channel::Channel(User *_owner_ptr, const string &_new_channel_name, const string&_new_channel_password) :
						_channel_name(_new_channel_name),
						_password(_new_channel_password),
						_is_topic_set(false),
						_invite_only_channel_flag(false),
						_moderated_channel_flag(false),
						_limited_users_on_channel(-1),
						_owner(_owner_ptr),
						_private_channel_flag(false),
						_secret_channel_flag(false),
						_topic_for_operators_flag(false),
						_no_messages_from_outside_channel_flag(false)
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

string &Channel::GetChannelName() {
	return (_channel_name);
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
			this->TakeAwayVoiceRights(_user_to_delete);
			this->DeleteInvite(_user_to_delete);
			return (0);
		}
	}
	return (1);
}

const vector<User *> &Channel::GetChannelUsers() const {
	return (_user_store);
}

const vector<User *> &Channel::GetChannelOperators() const {
	return (_operators);
}

void Channel::DeleteFromOperatorsNoPromo(User *_user_to_delete) {
	vector<User *>::iterator _curr_moderator = _operators.begin();
	vector<User *>::iterator _moderators_end = _operators.end();
	while (_curr_moderator != _moderators_end) {
		if (*_curr_moderator == _user_to_delete) {
			_operators.erase(_curr_moderator);
			return;
		}
		++_curr_moderator;
	}
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

int Channel::AddOperator(User *_new_operator) {
	_operators.push_back(_new_operator);
	return (0);
}

bool	Channel::IsInviteOnly() {
	return (_invite_only_channel_flag);
}

void Channel::SetInviteOnly(bool flag_condition) {
	if (_invite_only_channel_flag)
		_invites.clear();
	_invite_only_channel_flag = flag_condition;
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

bool Channel::IsTopicSet() {
	return (_is_topic_set);
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

bool Channel::IsOperator(User *_checked_user_prt) {
	vector<User *>::iterator _curr_operator = this->_operators.begin();
	vector<User *>::iterator _operators_end = this->_operators.end();
	while (_curr_operator != _operators_end) {
		if (*_curr_operator == _checked_user_prt) {
			return (true);
		}
		++_curr_operator;
	}
	return (false);
}

string Channel::NameReply() {
	string nameReplyString;
	vector<User *>::iterator _begin = _user_store.begin();
	vector<User *>::iterator _curr_user_ptr = _user_store.end();
	while (_curr_user_ptr != _begin) {
		--_curr_user_ptr;
		nameReplyString.append(" ");
		if(this->IsOperator(*_curr_user_ptr))
			nameReplyString.append("@");
		nameReplyString.append((*_curr_user_ptr)->GetUserNick());
	}
	return (nameReplyString);
}

bool Channel::IsModerated() {
	return (_moderated_channel_flag);
}

void Channel::SetIsModerated(bool statement) {
	_moderated_channel_flag = statement;
}

bool Channel::IsLimited() {
	if (_limited_users_on_channel < 0)
		return (false);
	return (true);
}

void Channel::SetIsLimited(int _limit) {
	_limited_users_on_channel = _limit;
}

int Channel::GetLimit() {
	return (_limited_users_on_channel);
}

bool Channel::IsLimitFull() {
	if ((int)_user_store.size() >= _limited_users_on_channel)
		return (true);
	return (false);
}

bool Channel::IsBanned(const string &_user_to_check) {
	vector<string>::iterator _curr_banned_user_name = _banned_users.begin();
	vector<string>::iterator _banned_user_names_end = _banned_users.end();
	while (_curr_banned_user_name != _banned_user_names_end) {
		if (_user_to_check == *_curr_banned_user_name)
			return (true);
		++_curr_banned_user_name;
	}
	return (false);
}

void Channel::BanUser(const string &_user_to_ban) {
	vector<string>::iterator _curr_banned_user_name = _banned_users.begin();
	vector<string>::iterator _banned_user_names_end = _banned_users.end();
	while (_curr_banned_user_name != _banned_user_names_end) {
		if (_user_to_ban == *_curr_banned_user_name)
			return;
		++_curr_banned_user_name;
	}
	_banned_users.push_back(_user_to_ban);
}

void Channel::UnBanUser(const string &_user_to_unban) {
	vector<string>::iterator _curr_banned_user_name = _banned_users.begin();
	vector<string>::iterator _banned_user_names_end = _banned_users.end();
	while (_curr_banned_user_name != _banned_user_names_end) {
		if (_user_to_unban == *_curr_banned_user_name) {
			_banned_users.erase(_curr_banned_user_name);
			return;
		}
		++_curr_banned_user_name;
	}
}

void Channel::SendToMembersFromUser(User &sender, const string message) {
	vector<User *>::iterator curr_user_ptr = _user_store.begin();
	vector<User *>::iterator user_ptrs_end = _user_store.end();
	string sending_message;
	sending_message += ':' + sender.GetUserNick() + " " + message + "\r\n";
	while (curr_user_ptr != user_ptrs_end) {
		std::cout << "FD " << (*curr_user_ptr)->GetUserFd() << " << " << sending_message <<std::endl;
		send((*curr_user_ptr)->GetUserFd(), sending_message.data(), sending_message.size(), 0);
		++curr_user_ptr;
	}
}

void Channel::SendBanListToUser(User &banlist_receiver) {
	vector<string>::iterator curr_banned_user = _banned_users.begin();
	vector<string>::iterator banned_users_end = _banned_users.end();
	string sending_message;
	while (curr_banned_user != banned_users_end) {
		sending_message += ":"SERVER_NAME" "CODE_TO_STRING(RPL_BANLIST)" " + this->_channel_name + " " + *curr_banned_user + "\r\n";
		std::cout << "FD " << banlist_receiver.GetUserFd() << " << " << sending_message <<std::endl;
		send(banlist_receiver.GetUserFd(), sending_message.data(), sending_message.size(), 0);
		sending_message.clear();
		++curr_banned_user;
	}
	sending_message += ":"SERVER_NAME" "CODE_TO_STRING(RPL_ENDOFBANLIST)" " + this->_channel_name + " :End of channel ban list" + "\r\n";
	std::cout << "FD " << banlist_receiver.GetUserFd() << " << " << sending_message <<std::endl;
}

void Channel::GiveVoiceRights(User *user_to_give_rights) {
	vector<User *>::iterator curr_user_can_talk_ptr = _can_talk_if_moderated.begin();
	vector<User *>::iterator user_can_talk_ptrs_end = _can_talk_if_moderated.end();
	while (curr_user_can_talk_ptr != user_can_talk_ptrs_end) {
		if (*curr_user_can_talk_ptr == user_to_give_rights)
			return;
		++curr_user_can_talk_ptr;
	}
	_can_talk_if_moderated.push_back(user_to_give_rights);
}

void Channel::TakeAwayVoiceRights(User *user_to_take_away_rights) {
	vector<User *>::iterator curr_user_can_talk_ptr = _can_talk_if_moderated.begin();
	vector<User *>::iterator user_can_talk_ptrs_end = _can_talk_if_moderated.end();
	while (curr_user_can_talk_ptr != user_can_talk_ptrs_end) {
		if (*curr_user_can_talk_ptr == user_to_take_away_rights) {
			_can_talk_if_moderated.erase(curr_user_can_talk_ptr);
			return;
		}
		++curr_user_can_talk_ptr;
	}
}

bool Channel::IsHasVoiceRights(User *user_to_check) {
	vector<User *>::iterator curr_user_can_talk_ptr = _can_talk_if_moderated.begin();
	vector<User *>::iterator user_can_talk_ptrs_end = _can_talk_if_moderated.end();
	while (curr_user_can_talk_ptr != user_can_talk_ptrs_end) {
		if (*curr_user_can_talk_ptr == user_to_check) {
			return (true);
		}
		++curr_user_can_talk_ptr;
	}
	return (false);
}

bool Channel::CheckPassword(const string &checked_password) {
	if (checked_password == this->_password)
		return (true);
	return (false);
}

void Channel::SetPassword(const string &new_password) {
	this->_password = new_password;
}

void Channel::RemovePassword() {
	this->_password.clear();
}