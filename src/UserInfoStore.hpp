//
// Created by ferun on 14.01.2022.
//

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
public:
	UserInfoStore();
	~UserInfoStore();
	bool	IsNickAvalable(const string _searching_nick) const;
	User	&CreateNewUser(int _new_user_fd);
	void	DeleteUser(User *_user_to_delete);
	void	DeleteChannel(string &_delete_channel_name);
	User	*FindUserByNick(string _searching_nick);

	/**
	 * Serach Users by there nickname in UserInfoStore.
	 * @Return 0 if every nickname is found in UserInfoStore.
	 * Pointers to Users cam be found in _result vector.
	 * @Return ERR_NOSUCHNICK if any nickname has no matches in UserInfoStore.
	 * All other nicknames/removed from _searching_nick.
	 */
	int		FindReceivers(vector<string> &_searching_receivers, vector<User *> &_result);
	Channel	*FindChannelByName(string _searching_channel_name);
	int		CreateNewChannel(User *_owner_ptr, const string &_new_channel_name, const string &_new_channel_password);
	void	PrintUserInfoStore();
};


#endif //FT_IRC_USERINFOSTORE_HPP
