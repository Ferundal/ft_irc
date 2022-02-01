#include "Parser.hpp"

Parser::Parser () {
    this->_commandList[0] = "ADMIN";
    this->_commandList[1] = "AWAY";
    this->_commandList[2] = "CONNECT";
    this->_commandList[3] = "DIE";
    this->_commandList[4] = "ERROR";
    this->_commandList[5] = "INFO";
    this->_commandList[6] = "INVITE";
    this->_commandList[7] = "ISON";
    this->_commandList[8] = "JOIN";
    this->_commandList[9] = "KICK";
    this->_commandList[10] = "KILL";
    this->_commandList[11] = "LINKS";
    this->_commandList[12] = "LIST";
    this->_commandList[13] = "LUSERS";
    this->_commandList[14] = "MODE";
    this->_commandList[15] = "MOTD";
    this->_commandList[16] = "MSG";
    this->_commandList[17] = "NAMES";
    this->_commandList[18] = "NICK";
    this->_commandList[19] = "NOTICE";
    this->_commandList[20] = "OPER";
    this->_commandList[21] = "PART";
    this->_commandList[22] = "PASS";
    this->_commandList[23] = "PING";
    this->_commandList[24] = "PONG";
    this->_commandList[25] = "PRIVMSG";
    this->_commandList[26] = "QUERY";
    this->_commandList[27] = "QUIT";
    this->_commandList[28] = "REHASH";
    this->_commandList[29] = "RESTART";
    this->_commandList[30] = "SERVICE";
    this->_commandList[31] = "SERVLIST";
    this->_commandList[32] = "SERVER";
    this->_commandList[33] = "SQUERY";
    this->_commandList[34] = "SQUIT";
    this->_commandList[35] = "STATS";
    this->_commandList[36] = "SUMMON";
    this->_commandList[37] = "TIME";
    this->_commandList[38] = "TOPIC";
    this->_commandList[39] = "TRACE";
    this->_commandList[40] = "USER";
    this->_commandList[41] = "USERHOST";
    this->_commandList[42] = "USERS";
    this->_commandList[43] = "VERSION";
    this->_commandList[44] = "WALLOPS";
    this->_commandList[45] = "WHO";
    this->_commandList[46] = "WHOIS";
    this->_commandList[47] = "WHOWAS";
}


Parser::~Parser () {}

Parser::Parser ( const Parser &other ) {
    operator = (other);
}

Parser  &Parser::operator = ( const Parser &other ) {
    if (this == &other)
        return *this;
    for (int i = 0; i < COMMAND_COUNT; ++i) {
        this->_commandList[i] = other._commandList[i];
    }
    // Some code in future
    return *this;
}

void Parser::errSendMsg(const char* er_code, User& user, const char* msg)
{
	std::string answer;
	answer = answer + ":" + SERVER_NAME + " " + er_code + " " + user.GetUserNick() + " " + msg + "\r\n";
	cout << answer << endl; // DEBUG out
	send(user.GetUserFd(), answer.data(), answer.size(), 0);
}

void Parser::rplSendMsg(const char* rpl_code, User& user, const char* msg)
{
	std::string answer;
	answer = answer + ":" + SERVER_NAME + " " + rpl_code + " " + user.GetUserNick() + " " + msg + "\r\n";
	cout << answer << endl; // DEBUG out
	send(user.GetUserFd(), answer.data(), answer.size(), 0);
}

void Parser::sendMsgToGroup(const string &sender, const char* rpl_code, const char *group, const vector<User *>& users, const char* msg) {
	vector<User *>::const_iterator _curr_user_ptr = users.begin();
	vector<User *>::const_iterator _user_ptrs_end = users.end();
	std::string reply;
	while (_curr_user_ptr != _user_ptrs_end) {
		reply.clear();
		reply = reply + ":" + sender + " " + rpl_code + " " + group + " :" + msg + "\r\n";
		cout << reply << endl; // DEBUG out
		send((*_curr_user_ptr)->GetUserFd(), reply.data(), reply.size(), 0);
		++_curr_user_ptr;
	}
}

std::string Parser::returnCommand ( std::string &str ) {
    std::string command;
    int         pos;

    pos = str.find(' ');
    if (pos < 0)
        return (str.substr(0, str.size() - 1));
    else
        return (str.substr(0, pos));
}

bool         Parser::checkCommand ( std::string &command ) {
    for (int i = 0; i < COMMAND_COUNT; ++i)
        if (this->_commandList[i] == command) {
            return true;
        }
    return false;
}

bool	IsChannel(const string &string_to_check) {
	if (string_to_check.data()[0] == '#')
		return (true);
	return (false);
}

int		StringToInt(const string &curr_string) {
	char	null_term_str[curr_string.size() + 1];

	null_term_str[curr_string.size()] = '\0';
	for (size_t i = 0; i < curr_string.size(); i++) {
		null_term_str[i] = curr_string[i];
	}
	return (atoi(null_term_str));
}

string ft_to_string_p(int num) {
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

int                Parser::countParam (std::string &str) {
  char        symb = '\r';
  int         n = 0;
  int         count = 0;

  if (str.find(":") != str.npos)
    symb = ':';

  while (str[n] != symb) {
    if (str[n] == ' ')
      ++count;
    ++n;
  }

  if (str[n] == ':' || str[n] == '\r')
    ++count;

  return count;
}

std::vector<std::string>    Parser::mySplit ( std::string str ) {
  int                       i = 0;
  char                      symb = ' ';
  int                       countWord = countParam(str);
  std::vector<std::string>  wordList(countWord);

  while (i < countWord) {
    if (i == countWord - 1)
        symb = '\r';
    wordList[i] = str.substr(0, str.find(symb));
    str.erase(0, str.find(symb) + 1);
    ++i;
  }

  return wordList;
}

void         Parser::commandUSER (ClientSocket &socket ) {
    std::vector<std::string>    paramList;
    std::string                 answer;


    paramList = mySplit(socket._msg_buff);

    // Check ERR_NEEDMOREPARAMS
    if (paramList.size() < 5) {
        errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
	    	(paramList[0] + " :Not enough parameters").data());
        return;
    }

    // Check ERR_ALREADYREGISTRED
    if (socket._usr_ptr->IsUserInfoSet() == true) {
        errSendMsg(CODE_TO_STRING(ERR_ALREADYREGISTRED), *socket._usr_ptr,
	    	(paramList[0] + " :You may not reregister").data());
        return;
    }
	if (paramList[2] == "*") {
		paramList[2] = inet_ntoa(socket._addr.sin_addr);
	}

	socket._usr_ptr->SetUserInfo(paramList[1], paramList[2], paramList[3], paramList[4]);
    if (socket._usr_ptr->SetActivated() == 0) {
		rplSendMsg(CODE_TO_STRING(RPL_MOTDSTART), *socket._usr_ptr,
			(answer + ":- " + SERVER_NAME + " answer of the day -").data());
		rplSendMsg(CODE_TO_STRING(RPL_MOTD), *socket._usr_ptr,
			(answer + ":- " + SERVER_NAME + " Welcome to the party -").data());
		rplSendMsg(CODE_TO_STRING(RPL_ENDOFMOTD), *socket._usr_ptr,
			(answer + ": " + SERVER_NAME + " End of /MOTD command").data());
	}
}

void		Parser::commandPASS (ClientSocket& socket){
	if (socket._usr_ptr->IsActivated())
		return;
	vector<string> paramList = mySplit(socket._msg_buff);
	if (paramList.size() < 2)
	{
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
	   		(paramList[0] + " :Not enough parameters").data());
		return;
	}
	string pass = paramList.at(1).erase(0,1);
	if(pass == GLOBAL_PASS)
		socket._usr_ptr->_is_password_checked = true;
	else{
		errSendMsg(CODE_TO_STRING(ERR_PASSWDMISMATCH), *socket._usr_ptr,
				   ":Password incorrect");
		return;
	}
	socket._usr_ptr->SetActivated();
}

void        Parser::commandNICK (ClientSocket &socket ) {
    std::vector<std::string>    paramList;
    std::string                 answer;
    std::string                 command;
    
    // Check ERR_NONICKNAMEGIVEN | if there are no nickname  in parameters
    paramList = mySplit(socket._msg_buff);
	if (countParam(socket._msg_buff) < 2) {
        errSendMsg(CODE_TO_STRING(ERR_NONICKNAMEGIVEN), *socket._usr_ptr,
	   		":No nickname given");
        return;
    }

    // Check ERR_ERRONEUSNICKNAME
    string s_ascii_err = "!\"#$%&\'()*+`-./:;<=>?@^_{}|~";
    for (size_t i = 0; i < paramList[1].size(); ++i)
    {
        if (s_ascii_err.find(paramList[1][i]) !=  string::npos)
            {
                errSendMsg(CODE_TO_STRING(ERR_ERRONEUSNICKNAME), *socket._usr_ptr,
			   		(paramList[1] + " :Erroneus nickname").data());
                return;
            }
    }
	// Check ERR_NICKCOLLISION
	if (paramList.size() <= 2) {
		if (socket._usr_ptr->SetNick(paramList[1]) != false) {
			errSendMsg(CODE_TO_STRING(ERR_NICKNAMEINUSE), *socket._usr_ptr,
					   (paramList[1] + " :Nickname is already in use").data());
			return;
		}
	}

	if(socket._usr_ptr->SetActivated() == 0)
	{
		rplSendMsg(CODE_TO_STRING(RPL_MOTDSTART), *socket._usr_ptr,
				   (answer + ":- " + SERVER_NAME + " answer of the day -").data());
		rplSendMsg(CODE_TO_STRING(RPL_MOTD), *socket._usr_ptr,
				   (answer + ":- " + SERVER_NAME + " Welcome to the party").data());
		rplSendMsg(CODE_TO_STRING(RPL_ENDOFMOTD), *socket._usr_ptr,
				   (answer + ":" + SERVER_NAME + " End of /MOTD command").data());
	}
}


void			Parser::commandPRIVMSG (ClientSocket &socket ){
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
	std::string command = paramList[0];

    // Checking ERR_NORECIPIENT
	int param_count = countParam(socket._msg_buff);

	if (param_count < 3) {
		errSendMsg(CODE_TO_STRING(ERR_NORECIPIENT),*socket._usr_ptr,
				   (":No recipient given ("+ command+ ")").data());
		return;
	}
	for (size_t i = 1; i < paramList.size(); ++i)
		for (size_t j = i + 1; j < paramList.size(); ++j)
			if (paramList[i] == paramList[j]){
				errSendMsg(CODE_TO_STRING(ERR_TOOMANYTARGETS),*socket._usr_ptr,
			   		(paramList[i] + " :Duplicate recipients. No message delivered").data());
				return;
			}

	for (size_t j = 1; j < paramList.size() - 1; ++j)
	{
		std::string sender = paramList[j];
		Channel* channel;
		std::string answer;
		if ((channel = socket._usr_ptr->ToStore().FindChannelByName(sender)) != NULL)
		{
			if ((channel->_no_messages_from_outside_channel_flag &&
			!socket._usr_ptr->IsMemberOfChannel(channel))
			||
			(channel->IsModerated() &&
			!channel->IsOperator(socket._usr_ptr) &&
			!channel->IsHasVoiceRights(socket._usr_ptr))
			||
			(!channel->IsOperator(socket._usr_ptr) &&
			channel->IsBanned(socket._usr_ptr->GetUserNick()))) {
				errSendMsg(CODE_TO_STRING(ERR_CANNOTSENDTOCHAN), *socket._usr_ptr,
						   (channel->GetChannelName() + " :Cannot send to channel").data());
				return;
			}
			vector<User*> usr_vector = channel->GetChannelUsers();
			for(size_t i = 0; i < usr_vector.size(); ++i)
			{
				if (socket._usr_ptr->GetUserNick() == usr_vector[i]->GetUserNick())
					continue;
				answer	= ":" + socket._usr_ptr->GetUserNick() + " PRIVMSG " + sender + " " + paramList[param_count - 1] + "\r\n";
				send(usr_vector[i]->GetUserFd(), answer.data(), answer.size(), 0);
				if (usr_vector[i]->IsAway())
					rplSendMsg(CODE_TO_STRING(RPL_AWAY), *socket._usr_ptr,
							   (usr_vector[i]->GetUserNick() + " :" + usr_vector[i]->GetAwayMessege()).data());
				cout << answer << endl; // DEBUG out
				answer.clear();
			}
		}
		else
		{
			User        *receiver = socket._usr_ptr->ToStore().FindUserByNick(sender);

			if (receiver == NULL) {
				errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr,
						   (sender + " :No such sender/channel").data());
				return;
			}

			answer	= ":" + socket._usr_ptr->GetUserNick() + " PRIVMSG " + sender + " " + paramList[param_count - 1] + "\r\n";
			send(receiver->GetUserFd(), answer.data(), answer.size(), 0);
			if (receiver->IsAway())
				rplSendMsg(CODE_TO_STRING(RPL_AWAY), *socket._usr_ptr,
						   (receiver->GetUserNick() + " :" +
						   receiver->GetAwayMessege()).data());
		}
	}
}

void	Parser::commandQUIT(ClientSocket& socket)
{
    void(socket._msg_buff);
	cout << "QUIT command done" << endl; // DEBUG out
	throw Parser::UserDeleteException();
}

void	Parser::commandWHOIS(ClientSocket& socket){
	string                      answer;
	std::vector<std::string>	paramList = mySplit(socket._msg_buff);
	std::string                 nick = paramList[1];
	User                        *user; // По кому ищется и отправляется инфа

    // Checking ERR_NONICKNAMEGIVEN
	int param_count = countParam(socket._msg_buff);
	if (param_count != 2) {
		errSendMsg(CODE_TO_STRING(ERR_NONICKNAMEGIVEN), *socket._usr_ptr,
	   		" :No nickname given");
		return;
	}

	if ((user = socket._usr_ptr->ToStore().FindUserByNick(paramList[1])) == NULL) {
        // Checking ERR_NOSUCHNICK
		errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr,
	   		(nick + " :No such nick/channel").data());
		return;
	}
    // Sending RPL_WHOISUSER
	rplSendMsg(CODE_TO_STRING(RPL_WHOISUSER), *socket._usr_ptr,
    	(answer + user->GetUserNick() + " " + user->GetUserName() + " " + user->GetUserHost() + " * " + user->GetUserRealName()).data());
	// Sending RPL_WHOISSERVER
	rplSendMsg(CODE_TO_STRING(RPL_WHOISSERVER), *socket._usr_ptr,
			   (answer + user->GetUserNick() + " " + user->GetServerName() + " : Worst Server Ever").data());
	// Sending RPL_WHOISCHANNELS
	vector<Channel *>::const_iterator curr_user_channel = user->GetChannels().begin();
	vector<Channel *>::const_iterator user_channels_end = user->GetChannels().end();
	answer.clear();
	answer =  user->GetUserNick() + " :";
	while (curr_user_channel != user_channels_end) {
		if (!(*curr_user_channel)->_secret_channel_flag || socket._usr_ptr->IsMemberOfChannel(*curr_user_channel)) {
			if ((*curr_user_channel)->IsOperator(user))
				answer += "@";
			else if ((*curr_user_channel)->IsHasVoiceRights(user))
				answer += "+";
			answer += (*curr_user_channel)->GetChannelName() + " ";
		}
		++curr_user_channel;
	}
	rplSendMsg(CODE_TO_STRING(RPL_WHOISCHANNELS), *socket._usr_ptr,
			   answer.data());
	answer.clear();
    rplSendMsg(CODE_TO_STRING(RPL_ENDOFWHOIS), *socket._usr_ptr,
    	(answer + user->GetUserNick() + " :End of /WHOIS list\r\n").data());

}

void	Parser::commandJOIN(ClientSocket& socket){
	std::vector<std::string>	paramList = mySplit(socket._msg_buff);
	std::string					command	= paramList[0];

	if(paramList.size() < 2)
	{
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
	    (command + " :Not enough parameters").data());
		return;
	}
	if (*(paramList[1].data()) != '#') {
		errSendMsg(CODE_TO_STRING(ERR_NOSUCHCHANNEL), *socket._usr_ptr,
				   (paramList[1] + " :No such channel").data());
		return;
	}
	int status;
	if (paramList.size() == 2)
		status = socket._usr_ptr->JoinChannel(paramList[1], "");
	else
		status = socket._usr_ptr->JoinChannel(paramList[1], paramList[2]);
	if (status == 0) {
		string answer;
		Channel *channel_ptr = socket._usr_ptr->ToStore().FindChannelByName(paramList[1]);
		vector<User *>::const_iterator _curr_channel_user = channel_ptr->GetChannelUsers().begin();
		vector<User *>::const_iterator _channel_users_end = channel_ptr->GetChannelUsers().end();
		while (_curr_channel_user != _channel_users_end) {
			answer.clear();
			answer = answer + ":" + socket._usr_ptr->GetUserFullName() + " JOIN " + paramList[1] + "\r\n";
			std::cout << answer << std::endl; // DEBUG outr
			send((*_curr_channel_user)->GetUserFd(), answer.data(), answer.size(), 0);
			++_curr_channel_user;
		}
		answer.clear();
		string topic;
		status = socket._usr_ptr->GetTopic(paramList[1], topic);
		if (status == 0) {
			rplSendMsg(CODE_TO_STRING(RPL_TOPIC), *socket._usr_ptr,
					   (paramList[1] + " :" + topic).data());
		} else {
			rplSendMsg(CODE_TO_STRING(RPL_NOTOPIC), *socket._usr_ptr,
					   (paramList[1] + " :No topic is set").data());
		}
//	353     RPL_NAMREPLY
		rplSendMsg(CODE_TO_STRING(RPL_NAMREPLY), *socket._usr_ptr,
			(answer + paramList[1] + " :" + channel_ptr->NameReply()).data()); // TODO добавить список ников
//	366     RPL_ENDOFNAMES
		rplSendMsg(CODE_TO_STRING(RPL_ENDOFNAMES), *socket._usr_ptr,
				   (answer + paramList[1] + " :End of /NAMES list").data());
	} else if (status == ERR_USERONCHANNEL) {
		errSendMsg(CODE_TO_STRING(ERR_USERONCHANNEL), *socket._usr_ptr,
				   (socket._usr_ptr->GetUserNick() + " " + paramList[1] + " :is already on channel").data());
	} else if (status == ERR_TOOMANYCHANNELS) {
		errSendMsg(CODE_TO_STRING(ERR_TOOMANYCHANNELS), *socket._usr_ptr,
				   (paramList[1] + " :You have joined too many channels").data());
	} else if (status == ERR_CHANNELISFULL) {
		errSendMsg(CODE_TO_STRING(ERR_CHANNELISFULL), *socket._usr_ptr,
				   (paramList[1] + " :Cannot join channel (+l)").data());
	} else if (status == ERR_BANNEDFROMCHAN) {
		errSendMsg(CODE_TO_STRING(ERR_BANNEDFROMCHAN), *socket._usr_ptr,
				   (paramList[1] + " :Cannot join channel (+b)").data());
	} else if (status == ERR_INVITEONLYCHAN) {
		errSendMsg(CODE_TO_STRING(ERR_INVITEONLYCHAN), *socket._usr_ptr,
				   (paramList[1] + " :Cannot join channel (+i)").data());
	} else if (status == ERR_BADCHANNELKEY) {
		errSendMsg(CODE_TO_STRING(ERR_BADCHANNELKEY), *socket._usr_ptr,
				   (paramList[1] + " :Cannot join channel (+k)").data());
	}
}


void 						Parser::commandAWAY( ClientSocket& socket ) {
    std::string                 answer;
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);

	if (paramList.size() == 1) {
		rplSendMsg(CODE_TO_STRING(RPL_UNAWAY), *socket._usr_ptr,
	    	":You are no longer marked as being away");
        socket._usr_ptr->SetNotAway();
    } else {
		rplSendMsg(CODE_TO_STRING(RPL_NOWAWAY), *socket._usr_ptr,
	    	":You have been marked as being away");
		if (!paramList[1].empty())
			paramList[1].erase(0, 1);
        socket._usr_ptr->SetAway(paramList[1]);
    }
}

void 						Parser::commandISON (ClientSocket& socket) {
	std::vector<std::string>    nicknameList = mySplit(socket._msg_buff);
    std::string                 command;
    std::string                 answer;

    command = returnCommand(socket._msg_buff);
	answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_ISON) + " " + socket._usr_ptr->GetUserNick();
	for (size_t i = 1; i < nicknameList.size(); ++i) {
		if (socket._usr_ptr->ToStore().FindUserByNick(nicknameList[i]) != NULL)
			answer += " " + nicknameList[i];
	}
	answer += "\r\n";
	std::cout << answer << std::endl; // DEBUG outr
	send(socket._fd, answer.data(), answer.size(), 0);

}

void 						Parser::commandLIST (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
	list<std::string>           listOfChanels;
    std::string                 answer;

    rplSendMsg(CODE_TO_STRING(RPL_LISTSTART), *socket._usr_ptr,
    	"Channel :Users  Name");;

    if (paramList.size() == 1) {
        listOfChanels = socket._usr_ptr->ChannelList("");
        list<std::string>::iterator currChanel= listOfChanels.begin();
        list<std::string>::iterator chanelsEnd= listOfChanels.end();
        while (currChanel != chanelsEnd) {
            answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_LIST) + " " + socket._usr_ptr->GetUserNick() + " " + *currChanel + "\r\n";
            send(socket._fd, answer.data(), answer.size(), 0);
            std::cout << answer << std::endl; // DEBUG out
            answer.clear();
            ++currChanel;
			answer.clear();
        }
    } else {
        for (size_t i = 0; i < paramList.size(); ++i) {
            listOfChanels = socket._usr_ptr->ChannelList(paramList[i]);
            if (!listOfChanels.empty()) {
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_LIST) + " " + socket._usr_ptr->GetUserNick() + " " + *listOfChanels.begin() + "\r\n";
                send(socket._fd, answer.data(), answer.size(), 0);
                answer.clear();
                std::cout << answer << std::endl; // DEBUG out
				answer.clear();
            }
        }
    }
    answer.clear();
    rplSendMsg(CODE_TO_STRING(RPL_LISTEND), *socket._usr_ptr,
   		":End of /LIST");
}

void 						Parser::commandPING (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
    std::cout << "PONG " << paramList[1] << std::endl; // DEBUG out
}

void 						Parser::commandWHO(ClientSocket& socket)
{
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
	std::string                 command;
	std::string                 answer;

//	352     RPL_WHOREPLY
	if (paramList.size() < 2 || !IsChannel(paramList[1])) {
		list<User *>::const_iterator curr_user = socket._usr_ptr->ToStore().GetAllActiveUsers().begin();
		list<User *>::const_iterator users_end = socket._usr_ptr->ToStore().GetAllActiveUsers().end();
		if (paramList.size() > 1) {
			while (curr_user != users_end) {
				if (!(*curr_user)->_is_hidden && *curr_user != socket._usr_ptr &&
						!socket._usr_ptr->IsOnSaneChannels(*curr_user)) {
						rplSendMsg(CODE_TO_STRING(RPL_WHOREPLY), *socket._usr_ptr,
								   ((*curr_user)->GetUserNick() + " "
								   + (*curr_user)->GetUserName() + " "
									+ (*curr_user)->GetUserHost() + " "
									+ (*curr_user)->GetServerName() + " "
									+ (*curr_user)->GetUserNick() + " * "
									+ (*curr_user)->GetUserRealName()).data());
					}
				}
				++curr_user;
			} else {
			while (curr_user != users_end) {
				if (!(*curr_user)->_is_hidden && *curr_user != socket._usr_ptr &&
					 !socket._usr_ptr->IsOnSaneChannels(*curr_user) &&
					 (paramList[1] == (*curr_user)->GetUserNick() ||
					 paramList[1] == (*curr_user)->GetUserName() ||
					 paramList[1] == (*curr_user)->GetUserHost() ||
					 paramList[1] == (*curr_user)->GetServerName() ||
					 paramList[1] == (*curr_user)->GetUserNick() ||
					 paramList[1] == (*curr_user)->GetUserRealName())) {
					rplSendMsg(CODE_TO_STRING(RPL_WHOREPLY), *socket._usr_ptr,
							   ((*curr_user)->GetUserNick() + " "
							   + (*curr_user)->GetUserName() + " "
							   + (*curr_user)->GetUserHost() + " "
							   + (*curr_user)->GetServerName() + " "
							   + (*curr_user)->GetUserNick() + " * "
							   + (*curr_user)->GetUserRealName()).data());
					}
				}
				++curr_user;
			}

	} else {
		Channel *channel = socket._usr_ptr->ToStore().FindChannelByName(
				paramList[1]);
		if (channel != NULL) {
			vector<User *> usr_vector = channel->GetChannelUsers();
			for (size_t i = 0; i < usr_vector.size(); ++i) {
				if (!usr_vector[i]->_is_hidden) {
					rplSendMsg(CODE_TO_STRING(RPL_WHOREPLY), *socket._usr_ptr,
							   (paramList[1] + " " +
								usr_vector[i]->GetUserName() +
								" "
								+ usr_vector[i]->GetUserHost() + " "
								+ usr_vector[i]->GetServerName() + " "
								+ usr_vector[i]->GetUserNick() + " * "
								+ usr_vector[i]->GetUserRealName()).data());
				}
			}
		}
	}
	rplSendMsg(CODE_TO_STRING(RPL_ENDOFWHO), *socket._usr_ptr,
			   (paramList[1] + " :End of /WHO list").data());
}

void 						Parser::commandINVITE (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
    std::string                 answer;

    // Checking NEEDMOREPARAM
    if (paramList.size() < 3) {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
        (paramList[0] + " :Not enough parameters").data());
        return;
    }

	Channel* channel_invite_to_ptr = socket._usr_ptr->ToStore().FindChannelByName(paramList[2]);
	if (channel_invite_to_ptr == NULL) {
		errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr,
				   (paramList[2] + " :No such nick/channel").data());
		return;
	}

	User*	reciever = socket._usr_ptr->ToStore().FindUserByNick(paramList[1]);
    // Checking ERR_NOSUCHNICK
    if (reciever == NULL) {
    	errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr,
        (paramList[1] + " :No such nick/channel").data());
        return;
    }

    // Checking ERR_NOTONCHANNEL
    if (!socket._usr_ptr->IsMemberOfChannel(channel_invite_to_ptr)) {
        errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr,
        (socket._usr_ptr->GetUserNick() + " :You're not on that channel").data());
        return;
    }

    // Checking ERR_USERONCHANNEL 443
    if (reciever->IsMemberOfChannel(channel_invite_to_ptr)) {
        errSendMsg(CODE_TO_STRING(ERR_USERONCHANNEL), *socket._usr_ptr, (paramList[1] + " " + paramList[2] + " :is already on channel").data());
        return;
    }

    // Checking ERR_CHANOPRIVSNEEDED 482
	if (channel_invite_to_ptr->IsInviteOnly() && !channel_invite_to_ptr->IsOperator(socket._usr_ptr)) {
        errSendMsg(CODE_TO_STRING(ERR_CHANOPRIVSNEEDED), *socket._usr_ptr, (paramList[2] + " :You're not channel operator").data());
        return;
    }
	channel_invite_to_ptr->AddInvite(reciever);
    rplSendMsg(CODE_TO_STRING(RPL_INVITING), *socket._usr_ptr,
    	(paramList[2] + " " + paramList[1]).data());

    answer = answer + ":" + socket._usr_ptr->GetUserNick() + " INVITE " + paramList[1] + " " + paramList[2] + "\r\n";
    cout << answer << endl;// DEBUG out
    send(reciever->GetUserFd(), answer.data(), answer.size(), 0);
}

void 						Parser::commandPART (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);

    if (paramList.size() >= 2) {
        for (size_t i = 1; i < paramList.size(); ++i) {
			string channel_name = paramList[i];
			string answer;
			Channel* channel = socket._usr_ptr->ToStore().FindChannelByName(channel_name);
			if (channel == NULL) {
				errSendMsg(CODE_TO_STRING(ERR_NOSUCHCHANNEL), *socket._usr_ptr,
						   (paramList[i] + " :No such channel").data());
				continue;
			}
			if (!socket._usr_ptr->IsMemberOfChannel(channel)) {
				errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr,
						   (paramList[i] + " :You're not on that channel").data());
				continue;
			}
            socket._usr_ptr->LeaveChannel(paramList[i]);
			channel = socket._usr_ptr->ToStore().FindChannelByName(channel_name);
			if (channel != NULL)
				channel->SendToMembersFromUser(*socket._usr_ptr, "PART " + channel_name);
        }
    } else {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
	    	(paramList[0] + " :Not enough parameters").data());
        return;
    }
}

void 						Parser::commandTOPIC (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);

	// Command need at least one param, send error if not enouth args
	if (paramList.size() < 2) {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
				   (paramList[0] + " :Not enough parameters").data());
		return;
	}
	// Sent info about channel name
	if (paramList.size() == 2) {
		std::string topic_store;
		int status = socket._usr_ptr->GetTopic(paramList[1], topic_store);
		if (status == 0) {
			topic_store = ":" + topic_store;
			rplSendMsg(CODE_TO_STRING(RPL_TOPIC), *socket._usr_ptr,
					topic_store.data());
		} else if (status == ERR_NOTONCHANNEL) {
			errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr,
					(paramList[1] + " :You're not on that channel").data());
		}
		else if (status == RPL_NOTOPIC) {
			rplSendMsg(CODE_TO_STRING(RPL_NOTOPIC), *socket._usr_ptr,
					(paramList[1] + " :No topic is set").data());
		}
		return;
	}
	//delete : from topic
	if (*paramList[2].data() == ':')
		paramList[2].erase(0, 1);
	//change current topic and send information about it to all users in channel
	if (paramList.size() == 3) {
		int status = socket._usr_ptr->ChangeTopic(paramList[1], paramList[2]);
		if (status == 0) {
			const vector<User *> &channel_users_ptr = socket._usr_ptr->ToStore().FindChannelByName(paramList[1])->GetChannelUsers();
			sendMsgToGroup(socket._usr_ptr->GetUserNick(),
						   "TOPIC",
						   paramList[1].data(), channel_users_ptr,
						   paramList[2].data());
		} else if (status == ERR_NOTONCHANNEL) {
			errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr,
					   (paramList[1] + " :You're not on that channel").data());
		} else if (status == ERR_CHANOPRIVSNEEDED) {
			errSendMsg(CODE_TO_STRING(ERR_CHANOPRIVSNEEDED), *socket._usr_ptr,
					   (paramList[1] + " :You're not channel operator").data());
		}
		return;
	}
}

void Parser::commandMODE(ClientSocket &socket) {
	std::vector<std::string> paramList = mySplit(socket._msg_buff);

	// Command need at least one param, send error if not enouth args
	if (paramList.size() < 3) {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
				   (paramList[0] + " :Not enough parameters").data());
		return;
	}
	int find_start = 0;
	bool turning_off_flag = false;
	if (paramList[2].find('+') == 0) {
		find_start = 1;
	}
	if (paramList[2].find('-') == 0) {
		turning_off_flag = true;
		find_start = 1;
	}
	string correct_flags = "opsitnmlbvkw";
	if (IsChannel(paramList.at(1))) {
		Channel *channel_ptr = socket._usr_ptr->ToStore().FindChannelByName(
				paramList.at(1));
		if (channel_ptr == NULL) {
			errSendMsg(CODE_TO_STRING(ERR_NOSUCHCHANNEL), *socket._usr_ptr,
					   (paramList[1] + " :No such channel").data());
			return;
		}
		if (!channel_ptr->IsOperator(socket._usr_ptr)) {
			errSendMsg(CODE_TO_STRING(ERR_CHANOPRIVSNEEDED), *socket._usr_ptr,
					   (paramList[1] + " :You're not channel operator").data());
			return;
		}
		if (correct_flags.find(paramList[2][find_start], 0) == string::npos) {
			errSendMsg(CODE_TO_STRING(ERR_UMODEUNKNOWNFLAG), *socket._usr_ptr,
					   (paramList[1] + " :Unknown MODE flag").data());
			return;
		}
		if (paramList[2][find_start] == 'o' ||
			paramList[2][find_start] == 'v') {

			if (paramList.size() < 4)
				return;
			User *target_user_ptr = socket._usr_ptr->ToStore().FindUserByNick(
					paramList[3]);
			if (target_user_ptr == NULL) {
				errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr,
						   (paramList[3] + " :No such nick/channel").data());
				return;
			}
			if (!target_user_ptr->IsMemberOfChannel(channel_ptr)) {
				errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr,
						   (target_user_ptr->GetUserNick() +
							" :No such nick/channel").data());
				return;
			}
			if (paramList[2][find_start] == 'o') {
				if (!turning_off_flag &&
					!channel_ptr->IsOperator(target_user_ptr)) {
					channel_ptr->AddOperator(target_user_ptr);
					channel_ptr->SendToMembersFromUser(*socket._usr_ptr,
													   "MODE " +
													   channel_ptr->GetChannelName() +
													   " +o " +
													   target_user_ptr->GetUserNick());
				}
				if (turning_off_flag &&
					channel_ptr->IsOperator(target_user_ptr)) {
					channel_ptr->DeleteFromOperatorsNoPromo(target_user_ptr);
					channel_ptr->SendToMembersFromUser(*socket._usr_ptr,
													   "MODE " +
													   channel_ptr->GetChannelName() +
													   " -o " +
													   target_user_ptr->GetUserNick());
				}
			} else {
				if (!turning_off_flag &&
					!channel_ptr->IsHasVoiceRights(target_user_ptr)) {
					channel_ptr->GiveVoiceRights(target_user_ptr);
					channel_ptr->SendToMembersFromUser(*socket._usr_ptr,
													   "MODE " +
													   channel_ptr->GetChannelName() +
													   " +v " +
													   target_user_ptr->GetUserNick());
				}
				if (turning_off_flag &&
					channel_ptr->IsHasVoiceRights(target_user_ptr)) {
					channel_ptr->TakeAwayVoiceRights(target_user_ptr);
					channel_ptr->SendToMembersFromUser(*socket._usr_ptr,
													   "MODE " +
													   channel_ptr->GetChannelName() +
													   " -v " +
													   target_user_ptr->GetUserNick());
				}
			}
		} else if (paramList[2][find_start] == 'p') {
			if (channel_ptr->_private_channel_flag && turning_off_flag) {
				channel_ptr->_private_channel_flag = false;
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " -p");
			}
			if (!channel_ptr->_private_channel_flag && !turning_off_flag) {
				channel_ptr->_private_channel_flag = true;
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " +p");
			}
		} else if (paramList[2][find_start] == 's') {
			if (channel_ptr->_secret_channel_flag && turning_off_flag) {
				channel_ptr->_secret_channel_flag = false;
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " -s");
			}
			if (!channel_ptr->_secret_channel_flag && !turning_off_flag) {
				channel_ptr->_secret_channel_flag = true;
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " +s");
			}
		} else if (paramList[2][find_start] == 'i') {
			if (channel_ptr->IsInviteOnly() && turning_off_flag) {
				channel_ptr->SetInviteOnly(false);
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " -i");
			}
			if (!channel_ptr->IsInviteOnly() && !turning_off_flag) {
				channel_ptr->SetInviteOnly(true);
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " +i");
			}
		} else if (paramList[2][find_start] == 't') {
			if (channel_ptr->_topic_for_operators_flag && turning_off_flag) {
				channel_ptr->_topic_for_operators_flag = false;
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " -t");
			}
			if (!channel_ptr->_topic_for_operators_flag && !turning_off_flag) {
				channel_ptr->_topic_for_operators_flag = true;
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " +t");
			}
		} else if (paramList[2][find_start] == 'n') {
			if (channel_ptr->_no_messages_from_outside_channel_flag &&
				turning_off_flag) {
				channel_ptr->_no_messages_from_outside_channel_flag = false;
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " -n");
			}
			if (!channel_ptr->_no_messages_from_outside_channel_flag &&
				!turning_off_flag) {
				channel_ptr->_no_messages_from_outside_channel_flag = true;
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " +n");
			}
		} else if (paramList[2][find_start] == 'm') {
			if (channel_ptr->IsModerated() && turning_off_flag) {
				channel_ptr->SetIsModerated(false);
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " -m");
			}
			if (!channel_ptr->IsModerated() && !turning_off_flag) {
				channel_ptr->SetIsModerated(true);
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " +m");
			}
		} else if (paramList[2][find_start] == 'l') {
			if (channel_ptr->IsLimited() && turning_off_flag) {
				channel_ptr->SetIsLimited(-1);
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " -l");
			}
			if (!channel_ptr->IsLimited() && !turning_off_flag &&
				paramList.size() > 3) {
				channel_ptr->SetIsLimited(StringToInt(paramList[3]));
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " +l " +
																	 ft_to_string_p(
																			 channel_ptr->GetLimit()));
			}
		} else if (paramList[2][find_start] == 'b') {
			if (turning_off_flag && paramList.size() > 3) {
				channel_ptr->UnBanUser(paramList[3]);
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " -b " +
																	 paramList[3]);
			}
			if (!turning_off_flag) {
				if (paramList.size() > 3) {
					channel_ptr->BanUser(paramList[3]);
					channel_ptr->SendToMembersFromUser(*socket._usr_ptr,
													   "MODE " +
													   channel_ptr->GetChannelName() +
													   " +b " + paramList[3]);
				} else {
					channel_ptr->SendBanListToUser(*socket._usr_ptr);
				}
			}
		} else if (paramList[2][find_start] == 'k') {
			if (turning_off_flag) {
				channel_ptr->RemovePassword();
			}
			if (!turning_off_flag && paramList.size() > 3) {
				channel_ptr->SetPassword(paramList[3]);
				channel_ptr->SendToMembersFromUser(*socket._usr_ptr, "MODE " +
																	 channel_ptr->GetChannelName() +
																	 " +k " +
																	 paramList[3]);
			}
		}
	} else {
		User *user_ptr = socket._usr_ptr->ToStore().FindUserByNick(
				paramList[1]);
		if (user_ptr == NULL) {
			errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr,
					   (paramList[1] + " :No such nick/channel").data());
			return;
		}
		if (socket._usr_ptr != user_ptr) {
			errSendMsg(CODE_TO_STRING(ERR_USERSDONTMATCH), *socket._usr_ptr,
					   (paramList[1] +
						" :Cant change mode for other users").data());
			return;
		}
		if (correct_flags.find(paramList[2][find_start], 0) == string::npos) {
			errSendMsg(CODE_TO_STRING(ERR_UMODEUNKNOWNFLAG), *socket._usr_ptr,
					   (paramList[1] + " :Unknown MODE flag").data());
			return;
		}
		if (paramList[2][find_start] == 'i') {
			if (turning_off_flag) {
				user_ptr->_is_hidden = false;
				rplSendMsg(CODE_TO_STRING(RPL_UMODEIS), *socket._usr_ptr,
						   ("MODE " + user_ptr->GetUserNick() + " -i").data());
			} else {
				user_ptr->_is_hidden = true;
				rplSendMsg(CODE_TO_STRING(RPL_UMODEIS), *socket._usr_ptr,
						   ("MODE " + user_ptr->GetUserNick() + " +i").data());
			}
		} else if (paramList[2][find_start] == 's') {
			if (turning_off_flag) {
				user_ptr->_is_receipt_server_notices = false;
				rplSendMsg(CODE_TO_STRING(RPL_UMODEIS), *socket._usr_ptr,
						   ("MODE " + user_ptr->GetUserNick() + " -s").data());
			} else {
				user_ptr->_is_receipt_server_notices = true;
				rplSendMsg(CODE_TO_STRING(RPL_UMODEIS), *socket._usr_ptr,
						   ("MODE " + user_ptr->GetUserNick() + " +s").data());
			}
		}
	}
}

void Parser::commandNOTICE(ClientSocket& socket){
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
	std::string command = paramList[0];
	int param_count = countParam(socket._msg_buff);
	if (param_count < 3){
		return;
	}

	std::string sender = paramList[1];
	Channel* channel;
	std::string answer;
	if ((channel = socket._usr_ptr->ToStore().FindChannelByName(sender)) != NULL)
	{
		if (channel->_no_messages_from_outside_channel_flag)
			return;
		vector<User*> usr_vector = channel->GetChannelUsers();
		for(size_t i = 0; i < usr_vector.size(); ++i)
		{
			if (socket._usr_ptr->GetUserNick() == usr_vector[i]->GetUserNick() || !usr_vector[i]->_is_receipt_server_notices || channel->IsBanned(socket._usr_ptr->GetUserNick()))
				continue;
			answer	= ":" + socket._usr_ptr->GetUserNick() + " NOTICE " + sender + " " + paramList[2] + "\r\n";
			send(usr_vector[i]->GetUserFd(), answer.data(), answer.size(), 0);
			cout << answer << endl; // DEBUG out
			answer.clear();
		}
	}
	else
	{
		User        *receiver = socket._usr_ptr->ToStore().FindUserByNick(sender);

		if (receiver == NULL || !receiver->_is_receipt_server_notices) {
			return;
		}
		answer	= ":" + socket._usr_ptr->GetUserNick() + " NOTICE " + sender + " " + paramList[2] + "\r\n";
		send(receiver->GetUserFd(), answer.data(), answer.size(), 0);
	}
}

void	Parser::commandKICK(ClientSocket& socket){
	vector<string> paramList = mySplit(socket._msg_buff);

	if(paramList.size() < 3){
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
				   (paramList[0] + " :Not enough parameters").data());
		return;
	}

	Channel* channel = socket._usr_ptr->ToStore().FindChannelByName(paramList[1]);

	if (channel == NULL){
		errSendMsg(CODE_TO_STRING(ERR_NOSUCHCHANNEL), *socket._usr_ptr,
   			(paramList[1] + " :No such channel").data());
		return;
	}
	if (socket._usr_ptr->IsMemberOfChannel(channel) == false){
		errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr,
	   		(paramList[1] + " :You're not on that channel").data());
		return;
	}
	if (channel->IsOperator(socket._usr_ptr) == false){
		errSendMsg(CODE_TO_STRING(ERR_CHANOPRIVSNEEDED), *socket._usr_ptr,
				   (paramList[1] + " :You're not channel operator").data());
		return;
	}

	User* receiver = socket._usr_ptr->ToStore().FindUserByNick(paramList[2]);

	if (receiver == NULL){
		errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr,
				   (paramList[2] + " :No such sender/channel").data());
		return;
	}
	if (!receiver->IsMemberOfChannel(channel)) {
		return;
	}

	string messege;
	messege += "KICK "  + paramList[1] + " " + paramList[2];
	if (paramList.size() == 3) {
		messege +=  + " :no reason given";
	} else if (paramList.size() > 3)
		messege += " " + paramList[3];
	channel->SendToMembersFromUser(*socket._usr_ptr, messege);
	socket._usr_ptr->ToStore().LeaveChannel(receiver, paramList[1]);
}

void    Parser::stringParser(ClientSocket &socket) {
	if (mySplit(socket._msg_buff).size() > 15 || (socket._msg_buff.size() - mySplit(socket._msg_buff)[0].size()) > 512) {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
	   	(mySplit(socket._msg_buff)[0] + " :Not enough parameters").data());
        return;
	}

    std::cout << socket._msg_buff << std::endl; //DEBUG out
    socket._msg_buff.erase(socket._msg_buff.size() - 1, 1);

    std::string command = returnCommand(socket._msg_buff);

    if (!checkCommand(command))
    {
    	errSendMsg(CODE_TO_STRING(ERR_UNKNOWNCOMMAND), *socket._usr_ptr,
	    	(command + " :Unknown command").data());
    	socket._msg_buff.clear();
    	return;
    }

    if (command == "USER") {
		commandUSER(socket);
    } else if (command == "NICK") {
		commandNICK(socket);
    } else if (command == "PASS") {
    	commandPASS(socket);
    } else if(!socket._usr_ptr->IsActivated()) {
    	return;
    } else if (command == "PRIVMSG") {
		commandPRIVMSG(socket);
    } else if (command == "QUIT") {
    	commandQUIT(socket);
    } else if (command == "WHOIS") {
    	commandWHOIS(socket);
    } else if (command == "JOIN") {
		commandJOIN(socket);
    } else if (command == "AWAY") {
    	commandAWAY(socket);
    } else if (command == "ISON") {
        commandISON(socket);
    } else if (command == "LIST") {
        commandLIST(socket);
    } else if (command == "PING") {
        commandPING(socket);
    } else if (command == "WHO") {
    	commandWHO(socket);
    } else if (command == "INVITE") {
        commandINVITE(socket);
    } else if (command == "PART") {
        commandPART(socket); 
    } else if (command == "TOPIC") {
		commandTOPIC(socket);
	} else if (command == "MODE") {
		commandMODE(socket);
	} else if (command == "NOTICE"){
		commandNOTICE(socket);
    } else if (command == "KICK"){
		commandKICK(socket);
    }
    socket._msg_buff.clear();
}