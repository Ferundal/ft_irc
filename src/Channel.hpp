#ifndef FT_IRC_CHANNEL_HPP
#define FT_IRC_CHANNEL_HPP
#include <list>
#include <string>
#include <sys/socket.h>
#include "User.hpp"
#include "UserInfoStore.hpp"
#include "Parser.hpp"

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
	bool			_invite_only_channel_flag;
	bool			_moderated_channel_flag;
	int				_limited_users_on_channel;
	User			*_owner;
	vector<User *>	_operators;
	vector<User *>	_invites;
	vector<User *>	_can_talk_if_moderated;
	vector<User *>	_user_store;
	vector<string>	_banned_users;
public:
	bool			_private_channel_flag;
	bool			_secret_channel_flag;
	bool			_topic_for_operators_flag;
	bool			_no_messages_from_outside_channel_flag;
private:
	/*
	 *
	 */
	void AddUser(User &_new_user);
	void DeleteFromOperators(User *_user_to_delete);
	int DeleteUser(User *_user_to_delete);

public:
	void DeleteFromOperatorsNoPromo(User *_user_to_delete);
	string &GetChannelName();
	const vector<User *> &GetChannelUsers() const;
	const vector<User *> &GetChannelOperators() const;
	Channel(User *_owner_ptr, const string &_new_channel_name, const string &_new_channel_password);
	~Channel();

	int AddOperator(User *_new_operator);

	void SetInviteOnly(bool flag_condition);
	bool IsInviteOnly();
	/**
	 * Check is _checked_user_ptr user invited.
	 * @Rerurn true if invited.
	 * @Return false if not.
	 */
	bool IsInvited(User *_checked_user_ptr);
	void AddInvite(User *_new_invite_user_ptr);

	bool IsTopicSet();
	bool IsModerated();
	void SetIsModerated(bool statement);
	bool IsLimited();
	void SetIsLimited(int _limit);
	int	GetLimit();
	bool IsLimitFull();
	bool IsBanned(const string &_user_to_check);
	void BanUser(const string &_user_to_ban);
	void UnBanUser(const string &_user_to_ban);
	void SendToMembersFromUser(User &sender, const string _message);

	void GiveVoiceRights(User *user_to_give_rights);
	void TakeAwayVoiceRights(User *user_to_take_away_rights);
	bool IsHasVoiceRights(User *user_to_check);



	/*
	 * Delete invite for _checked_user_ptr if it is exist.
	 */
	void DeleteInvite(User *_user_for_invitation_delete);

	bool IsOperator(User *_checked_user_prt);
	string NameReply();
	void SendBanListToUser(User &sender);

	bool CheckPassword(const string &checked_password);
	void SetPassword(const string &new_password);
	void RemovePassword();

};


#endif //FT_IRC_CHANNEL_HPP
