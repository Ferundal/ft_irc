//
// Created by ferun on 14.01.2022.
//

#ifndef FT_IRC_USER_HPP
#define FT_IRC_USER_HPP
#include <string>
#include <vector>
#include <exception>
#include "Channel.hpp"
#include "UserInfoStore.hpp"

using std::string;
using std::vector;
using std::list;

class Channel;

class User {
	friend class Channel;
	friend class UserInfoStore;
private:
	int					_fd;
	bool				_is_activated;
	string				_user_name;
	string				_host_name;
	string				_server_name;
	string				_real_name;
	string				_nick;
	bool				_is_;
	bool				_is_away;
	string				_away_message;
	UserInfoStore		*_to_user_store;
	vector<Channel *>	_membership;
private:
	User(int _new_fd);
	int CancelChannelMembership(Channel *_cannel_to_leave);
public:
	/**
	 *  Return User's nickname.
	 */
	const string &GetUserNick() const;

	/**
	 *  Return User's connection fd.
	 */
	const int &GetUserFd() const;

	/**
	 *  Return User's connection username.
	 */
	const string &GetUserName() const;

	/**
	 *  Return User's connection host.
	 */
	const string &GetUserHost() const;

	/**
	 *  Return User's connection realname.
	 */
	const string &GetUserRealName() const;

	/**
	 *  Set user nick to database.
	 *  @Return 0 if success.
	 *  @Return 1 if nick is already used
	 */
	int SetNick(const string &_new_nick);

	/**
	 *  Set user information.
	 *  @Return 0 if success.
	 *  @Return 1 if information already set.
	 */
	int SetUserInfo(const string &_new_user_name,
					const string &_new_host_name,
					const string &_new_server_name,
					const string &_new_real_name);

	/**
	 *  Mark user as active.
	 *  @Return 0 if success.
	 *  @Return 1 if User has not enough information to be active.
	 */
	int SetActivated(void);

	void SetAway(const string &_new_away_message);
	void SetNotAway(void);

	/**
	 *  Give information about user registration status.
	 *  @Return true if user is mark as active.
	 *  @Return false if not.
	 */
	bool IsActivated() const;

	bool IsAway() const;

	bool IsMemberOfChannel(Channel *_channel_ptr);

	/**
	 *  Check is user info alreay set.
	 *  @Return true if user info is already set.
	 *  @Return false if not.
	 */
	bool IsUserInfoSet() const;

	/**
	 *  Leave channel with _channel_name.
	 *  @Return 0 if success.
	 *  @Return 1 if User is not a member of _channel_name.
	 */
	int LeaveChannel(const string &_channel_name);

	/**
	 *  Create name of the user for server in server responce format
	 */
	string GetUserFullName(void) const;

	/**
	 * Return UserInfoStore what current user belongs to
	 */
	UserInfoStore &ToStore(void);

	/**
	 * Try to join channel with _channel_name.
	 * @Return 0 if success.
	 * @Return IRC error cod for every problem.
	 */
	int JoinChannel(const string &_channel_name, const string &_channel_password);

	/**
	 * Get information about channels visible to the User
	 * @Return list with send ready string information.
	 */
	list<string> ChannelList(const string &_channel_name);

	int SendInvite(const string &_invited_user_nick, const string &_channel_invite_to);

	int ChangeTopic(const string &_channel_to_change_topic, const string &_new_topic);
};


#endif //FT_IRC_USER_HPP
