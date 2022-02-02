#ifndef FT_IRC_USERINFOSTORE_HPP
#define FT_IRC_USERINFOSTORE_HPP
#include <list>
#include <iostream>
#include "User.hpp"
#include "Channel.hpp"
#include "error_answers.hpp"


using std::list;
using std::string;
using std::vector;

class User;
class Channel;

class UserInfoStore {
	friend class User;
	friend class Channel;
private:
	list<User> _users_store;
	list<User *> _connected_users;
	list<Channel> _active_channels;
private:
	void	DeleteChannel(const string &_delete_channel_name);
public:
	UserInfoStore();
	~UserInfoStore();
	bool	IsNickAvalable(const string _searching_nick) const;
	User	&CreateNewUser(int _new_user_fd);
	void	DeleteUser(User *_user_to_delete);
	User	*FindUserByNick(string _searching_nick);

	Channel	*FindChannelByName(string _searching_channel_name);
	int		CreateNewChannel(User *_owner_ptr, const string &_new_channel_name, const string &_new_channel_password);
	int		LeaveChannel(User *_member_user_ptr, const string  &_channel_to_leave);
	void	PrintUserInfoStore();
	const list<User *>	&GetAllActiveUsers() const;
};


#endif //FT_IRC_USERINFOSTORE_HPP
