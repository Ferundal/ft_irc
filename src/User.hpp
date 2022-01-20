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

class Channel;

class User {
	friend class Channel;
	friend class UserInfoStore;
private:
	int					_fd;
	bool				_is_active;
	string				_user_name;
	string				_host_name;
	string				_server_name;
	string				_real_name;
	string				_nick;
	UserInfoStore		*_user_store;
	vector<Channel *>	_membership;
private:
	User();
public:

	/*
	 *  Set user nick to database.
	 *  Return 0 if success.
	 *  Return 1 if nick is already used
	 */
	int SetNick(const string &_new_nick);

	/*
	 *  Set user information.
	 *  Return 0 if success.
	 *  Return 1 if information already set.
	 */
	int SetUserInfo(const string &_new_user_name,
					const string &_new_host_name,
					const string &_new_server_name,
					const string &_new_real_name);

	/*
	 *  Mark user as active.
	 *  Return 0 if success.
	 *  Return 1 if User has not enough information to be active.
	 */
	int SetActive(void);

	/*
	 *  Give information about user registration status.
	 *  Return true if user is mark as active.
	 *  Return false if not.
	 */
	bool IsActive();

	/*
	 *  Join channel with _channel_name.
	 *  If _channel_name Channel do not exist - create new one.
	 *  Return 0 if success.
	 *  Return 1 if _channel_name exist and has different password.
	 */
	int JoinChannel(const string &_channel_name);

	/*
	 *  Leave channel with _channel_name.
	 *  Return 0 if success.
	 *  Return 1 if User is not a member of _channel_name.
	 */
	int LeaveChannel(const string &_channel_name);
};


#endif //FT_IRC_USER_HPP
