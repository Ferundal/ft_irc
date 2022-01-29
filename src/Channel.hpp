//
// Created by ferun on 14.01.2022.
//

#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP
#include <list>
#include <string>
#include "User.hpp"
#include "UserInfoStore.hpp"

using std::vector;
using std::string;

class User;
class UserInfoStore;

class Channel {
	friend class User;
	friend class UserInfoStore;
private:
	string			_channel_name;
	string			_password;
	bool			_is_topic_set;
	string			_channel_topic;
	bool			_private_channel_flag;
	bool			_secret_channel_flag;
	bool			_invite_only_channel_flag;
	bool			_topic_for_operators_flag;
	bool			_no_messages_from_outside_channel_flag;
	bool			_moderated_channel_flag;
	int				_limited_users_on_channel;
	User			*_owner;
	vector<User *>	_operators;
	vector<User *>	_invites;
	vector<User *>	_user_store;
private:
	/*
	 *
	 */
	void AddUser(User &_new_user);
	void DeleteFromOperators(User *_user_to_delete);
	int DeleteUser(User *_user_to_delete);

public:
	const vector<User *> &GetChannelUsers() const;
	Channel(User *_owner_ptr, const string &_new_channel_name, const string &_new_channel_password);
	~Channel();
	void AddInvite(User *_new_invite_user_ptr);

	/**
	 * Check is _checked_user_ptr user invited.
	 * @Rerurn true if invited.
	 * @Return false if not.
	 */
	bool IsInvited(User *_checked_user_ptr);


	bool IsTopicSet();
	/*
	 * Delete invite for _checked_user_ptr if it is exist.
	 */
	void DeleteInvite(User *_user_for_invitation_delete);

	bool IsOperator(User *_checked_user_prt);
};


#endif //FT_IRC_CHANNEL_HPP
