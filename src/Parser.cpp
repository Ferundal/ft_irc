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

void Parser::rplSendMsgFrom(const string &sender, const char* rpl_code, User& user, const char* msg)
{
	std::string answer;
	answer = answer + ":" + sender + " " + rpl_code + " " + user.GetUserNick() + " " + msg + "\r\n";
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

void         Parser::commandUSER (ClientSocket &socket ) { // TODO пересмотреть логику ошибок
	// TODO Checking repeat NICK and USER in DB
    bool                        isSetUserInfo;
    std::vector<std::string>    paramList;
    std::string                 answer;


    paramList = mySplit(socket._msg_buff);

    // Check ERR_NEEDMOREPARAMS
    if (paramList.size() != 5) {
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

	if (socket._usr_ptr->IsActivated() == false) {
        isSetUserInfo = socket._usr_ptr->SetUserInfo(paramList[1], paramList[2], paramList[3], paramList[4]);
        if (isSetUserInfo == false) { //USER NePess * 127.0.0.1 :Shuchu Pes
            if (socket._usr_ptr->SetActivated() == 0) {
            	rplSendMsg(CODE_TO_STRING(RPL_MOTDSTART), *socket._usr_ptr,
			    	(answer + ":- " + SERVER_NAME + " answer of the day -").data());
            	rplSendMsg(CODE_TO_STRING(RPL_MOTD), *socket._usr_ptr,
			    	(answer + ":- " + SERVER_NAME + " Welcome to the party -").data());
            	rplSendMsg(CODE_TO_STRING(RPL_ENDOFMOTD), *socket._usr_ptr,
			    	(answer + ": " + SERVER_NAME + " End of /MOTD command").data());
            }
        }
    }
}

void        Parser::commandNICK (ClientSocket &socket ) {
	// TODO Checking repeat NICK and USER in DB
    bool                        isSetNickInfo;
    int                         checker = 0;
    std::vector<std::string>    paramList;
    std::string                 answer;
    std::string                 command;
    
    // Check ERR_NONICKNAMEGIVEN | if there are no nickname  in parameters
    paramList = mySplit(socket._msg_buff);

    for (size_t i = 0; i < paramList.size(); ++i) {
        if (checkCommand(paramList[i]) == true && countParam(socket._msg_buff) == 2)
            ++checker;
    }
    if (checker != 1) {
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
    if (paramList.size() == 2) {
        if (socket._usr_ptr->SetNick(paramList[2]) != false) {
            errSendMsg(CODE_TO_STRING(ERR_NICKCOLLISION), *socket._usr_ptr,
		   		(paramList[1] + " :Nickname collision KILL").data());
            return;
    } else if (paramList.size() == 3) { //Check ERR_NICKNAMEINUSE
        if (socket._usr_ptr->SetNick(paramList[2]) != false) {
            errSendMsg(CODE_TO_STRING(ERR_NICKNAMEINUSE), *socket._usr_ptr,
		   		(paramList[2] + " :Nickname is already in use").data());
            return;
        }
    }

    

	if (socket._usr_ptr->IsActivated() == false) {
        isSetNickInfo = socket._usr_ptr->SetNick(paramList[1]);
        if (isSetNickInfo == false) {
            // std::cout << "|INFO| [Nick successfuly added]" << std::endl;
            if (socket._usr_ptr->SetActivated() == 0) {
            	rplSendMsg(CODE_TO_STRING(RPL_MOTDSTART), *socket._usr_ptr,
			    	(answer + ":- " + SERVER_NAME + " answer of the day -").data());
            	rplSendMsg(CODE_TO_STRING(RPL_MOTD), *socket._usr_ptr,
			    	(answer + ":- " + SERVER_NAME + " Welcome to the party").data());
            	rplSendMsg(CODE_TO_STRING(RPL_ENDOFMOTD), *socket._usr_ptr,
			    	(answer + ":" + SERVER_NAME + " End of /MOTD command").data());
            }
        }
    }
}
}


void    Parser::commandPRIVMSG (ClientSocket &socket ) {


        // ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
        // ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
        // ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
        // ERR_NOSUCHNICK
        // RPL_AWAY



	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
	std::string command = paramList[0];

    // Checking ERR_NORECIPIENT
	int param_count = countParam(socket._msg_buff);
	if (param_count < 3) {
		errSendMsg(CODE_TO_STRING(ERR_NORECIPIENT),*socket._usr_ptr,
				   (":No recipient given ("+ command+ ")").data());
		return;
	}

	// TODO реализовать отправку по всем никнеймам?
	std::string sender = paramList[1]; // TODO проверка параметра на имя ника или канала  - разделить
	Channel* channel;
	std::string answer;
	if ((channel = socket._usr_ptr->ToStore().FindChannelByName(sender)) != NULL)
	{
		vector<User*> usr_vector = channel->GetChannelUsers();
		for(size_t i = 0; i < usr_vector.size(); ++i)
		{
			if (socket._usr_ptr->GetUserNick() == usr_vector[i]->GetUserNick())
				continue;
			answer	= ":" + socket._usr_ptr->GetUserNick() + " PRIVMSG " + sender + " " + paramList[2] + "\r\n";
			send(usr_vector[i]->GetUserFd(), answer.data(), answer.size(), 0);
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

		answer	= ":" + socket._usr_ptr->GetUserNick() + " PRIVMSG " + sender + " " + paramList[2] + "\r\n";
		send(receiver->GetUserFd(), answer.data(), answer.size(), 0);
	}

//	ERR_NORECIPIENT(Ok)             ERR_NOTEXTTOSEND
//	ERR_CANNOTSENDTOCHAN			ERR_NOTOPLEVEL
//	ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
//	ERR_NOSUCHNICK(Ok)
//	RPL_AWAY
}

void	Parser::commandQUIT(ClientSocket& socket)
{
    void(socket._msg_buff);
	cout << "QUIT command done" << endl; // DEBUG out
	throw Parser::UserDeleteException();
}


        //   ERR_NOSUCHSERVER [OK]           ERR_NONICKNAMEGIVEN [OK]
        //   RPL_WHOISUSER [OK]              RPL_WHOISCHANNELS [In process]
        //   RPL_WHOISCHANNELS [In process]  RPL_WHOISSERVER [OK]
        //   RPL_AWAY [OK]                   RPL_WHOISOPERATOR [OK]
        //   RPL_WHOISIDLE [OK]              ERR_NOSUCHNICK [OK]
        //   RPL_ENDOFWHOIS [OK]



void	Parser::commandWHOIS(ClientSocket& socket){
	string                      answer;
	std::vector<std::string>	paramList = mySplit(socket._msg_buff);
	std::string                 nick = paramList[1];
	User                        *user; // По кому ищется и отправляется инфа

    // Checking ERR_NONICKNAMEGIVEN
	int param_count = countParam(socket._msg_buff);
	if (param_count != 2) {
		errSendMsg(CODE_TO_STRING(ERR_NONICKNAMEGIVEN), *socket._usr_ptr,
	   		":No nickname given");
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
    // Sending RPL_ENDOFWHOIS
    rplSendMsg(CODE_TO_STRING(RPL_ENDOFWHOIS), *socket._usr_ptr,
    	(answer + user->GetUserNick() + " :End of /WHOIS list\r\n").data());

}

void	Parser::commandJOIN(ClientSocket& socket){
	std::vector<std::string>	paramList = mySplit(socket._msg_buff);
	std::string					command	= paramList[0];
	//int							param_count = countParam(socket._msg_buff);

	if(paramList.size() < 2)
	{
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
	    (command + " :Not enough parameters").data());
		return;
	}
	int status;
	if (paramList.size() == 2)
		status = socket._usr_ptr->JoinChannel(paramList[1], "");
	else
		status = socket._usr_ptr->JoinChannel(paramList[1], paramList[2]);
	if (status == 0) {
		string answer;
		answer = answer + ":" + socket._usr_ptr->GetUserNick() + " JOIN " +
				 paramList[1] + "\r\n";
		cout << answer << endl;// DEBUG out
		send(socket._fd, answer.data(), answer.size(), 0);
		answer.clear();
		Channel *channel_ptr = socket._usr_ptr->ToStore().FindChannelByName(paramList[1]);
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
//	"<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"  RPL_ENDOFNAMES
//	socket._usr_ptr->ToStore().FindChannelByName();
		rplSendMsg(CODE_TO_STRING(RPL_NAMREPLY), *socket._usr_ptr,
				   (answer + paramList[1] + " :" + channel_ptr->NameReply()).data()); // TODO добавить список ников

//	366     RPL_ENDOFNAMES
//	"<channel> :End of /NAMES list"
		rplSendMsg(CODE_TO_STRING(RPL_ENDOFNAMES), *socket._usr_ptr,
				   (answer + paramList[1] + " :End of /NAMES list").data());

	} else if (status == ERR_INVITEONLYCHAN) {
		errSendMsg(CODE_TO_STRING(ERR_INVITEONLYCHAN), *socket._usr_ptr,
				   (paramList[1] + " :Cannot join channel (+i)").data());
	} else if (status == ERR_BADCHANNELKEY) {
		errSendMsg(CODE_TO_STRING(ERR_BADCHANNELKEY), *socket._usr_ptr,
				   (paramList[1] + " :Cannot join channel (+k)").data());
	}
//	ERR_NEEDMOREPARAMS(Ok)          ERR_BANNEDFROMCHAN
//	ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//	ERR_CHANNELISFULL               ERR_BADCHANMASK
//	ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//	RPL_TOPIC
//	RPL_NAMREPLY
// <nick> <user> <host> * :<real name>
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
        socket._usr_ptr->SetAway(paramList[1]);
    }
}


void 						Parser::commandISON (ClientSocket& socket) {
	std::vector<std::string>    nicknameList = mySplit(socket._msg_buff);
    std::string                 command;
    std::string                 answer;

    command = returnCommand(socket._msg_buff);
    if (socket._msg_buff.size() - command.size() <= 512) {
        answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_ISON) + " " + socket._usr_ptr->GetUserNick();
        for (size_t i = 1; i < nicknameList.size(); ++i) {
            if (socket._usr_ptr->ToStore().FindUserByNick(nicknameList[i]) != NULL)
                answer += " " + nicknameList[i];
        }
        answer += "\r\n";
        std::cout << answer << std::endl; // DEBUG outr
        send(socket._fd, answer.data(), answer.size(), 0);
    } else {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
	   		(nicknameList[0] + " :Not enough parameters").data());
        return;
    }
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
            ++currChanel;
        }
    } else {
        for (size_t i = 0; i < paramList.size(); ++i) {
            listOfChanels = socket._usr_ptr->ChannelList(paramList[i]);
            if (!listOfChanels.empty()) {
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_LIST) + " " + socket._usr_ptr->GetUserNick() + " " + *listOfChanels.begin() + "\r\n";
                send(socket._fd, answer.data(), answer.size(), 0);
                std::cout << answer << std::endl; // DEBUG out
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
//	"<channel> <user> <host> <server> <nick> \
//  <H|G>[*][@|+] :<hopcount> <real name>"
	rplSendMsg(CODE_TO_STRING(RPL_WHOREPLY), *socket._usr_ptr,
		(paramList[1] + " Psina * 127.0.0.1 archie0 @ :Luto").data());
	rplSendMsg(CODE_TO_STRING(RPL_WHOREPLY), *socket._usr_ptr,
		(paramList[1] + " Dubin * 127.0.0.1 lololo * :kek").data());
//		(paramList[1] + " has 2 users. Operator: archie0").data());
//"352 * " + channel + " has " + usercount + " users. Operator: " + operator + "\r\n"


//  315     RPL_ENDOFWHO
//  "<name> :End of /WHO list"
	rplSendMsg(CODE_TO_STRING(RPL_ENDOFWHO), *socket._usr_ptr,
		(paramList[1] + " :End of /WHO list").data());

// INFO kreker - nickname  NePess - username * - hostname 127.0.0.1 - servername :Shuchu Pes - realname


//  ERR_NOSUCHSERVER
}

void 						Parser::commandINVITE (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
    std::string                 answer;

    // Checking NEEDMOREPARAM
    if (paramList.size() != 3) {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
        (paramList[0] + " :Not enough parameters").data());
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
    if (socket._usr_ptr->LeaveChannel(paramList[2]) == ERR_NOTONCHANNEL) {
        errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr,
        (paramList[2] + " :You're not on that channel").data());
        return;
    }

//    // Checking ERR_USERONCHANNEL 443   <--- Not work
//    if (socket._usr_ptr) {
//        errSendMsg(CODE_TO_STRING(ERR_USERONCHANNEL), *socket._usr_ptr, (paramList[1] + " " + paramList[2] + " :is already on channel").data());
//        return;
//    }
//
//    // Checking ERR_CHANOPRIVSNEEDED 482  <---  Not work
//        if (socket._usr_ptr) {
//        errSendMsg(CODE_TO_STRING(ERR_CHANOPRIVSNEEDED), *socket._usr_ptr, (paramList[2] + " :You're not channel operator").data());
//        return;
//    }

    rplSendMsg(CODE_TO_STRING(RPL_INVITING), *socket._usr_ptr,
    	(paramList[2] + " " + paramList[1]).data());

    answer = answer + ":" + socket._usr_ptr->GetUserNick() + " INVITE " + paramList[1] + " " + paramList[2] + "\r\n";
    cout << answer << endl;// DEBUG out
    send(reciever->GetUserFd(), answer.data(), answer.size(), 0);
}

void 						Parser::commandPART (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);

    // [CHECK] There are no same Chanels in ListOfChanels
    for (size_t i = 1; i < paramList.size(); ++i) {
        if ((socket._usr_ptr->ToStore().FindChannelByName(paramList[i]) == NULL)) {
		    errSendMsg(CODE_TO_STRING(ERR_NOSUCHCHANNEL), *socket._usr_ptr,
		   		(paramList[i] + " :No such channel").data());
            return;
        }
    }

    // [CHECK] There are more than 1 parameters in ListOfParameters
    if (paramList.size() >= 2) {
        for (size_t i = 1; i < paramList.size(); ++i) {
			string channel_name = paramList[1];
			string answer;
			Channel* channel = socket._usr_ptr->ToStore().FindChannelByName(channel_name);
			vector<User*> usr_vector = channel->GetChannelUsers();
			for(size_t i = 0; i < usr_vector.size(); ++i)
			{
				if (socket._usr_ptr->GetUserNick() == usr_vector[i]->GetUserNick())
					continue;
				answer	= ":" + socket._usr_ptr->GetUserNick() + " PART " + channel_name + "\r\n";
				send(usr_vector[i]->GetUserFd(), answer.data(), answer.size(), 0);
				cout << answer << endl; // DEBUG out
				answer.clear();
			}
            if(socket._usr_ptr->LeaveChannel(paramList[i]) == ERR_NOTONCHANNEL) {
    // [CHECK] There is not on chanel
        		errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr,
			   		(paramList[i] + " :You're not on that channel").data());
                return;
            }
        }
    } else {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,
	    	(paramList[0] + " :Not enough parameters").data());
        return;
    }

	string channel_name = paramList[1];
    string answer;
    Channel* channel = socket._usr_ptr->ToStore().FindChannelByName(channel_name);
	vector<User*> usr_vector = channel->GetChannelUsers();

	for(size_t i = 0; i < usr_vector.size(); ++i)
	{
		if (socket._usr_ptr->GetUserNick() == usr_vector[i]->GetUserNick())
			continue;
		answer	= ":" + socket._usr_ptr->GetUserNick() + " PART " + channel_name + "\r\n";
		send(usr_vector[i]->GetUserFd(), answer.data(), answer.size(), 0);
		cout << answer << endl; // DEBUG out
		answer.clear();
	}
}


        //   ERR_NEEDMOREPARAMS [OK]              ERR_NOTONCHANNEL [ok]
        //   RPL_NOTOPIC [OK]                     RPL_TOPIC [OK]
        //   ERR_CHANOPRIVSNEEDED [OK]

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

void    Parser::stringParser(ClientSocket &socket) {
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

    if (command == "USER") { //             ERR: 2, RPL: 0  [OK][OK]
		commandUSER(socket);
    } else if (command == "NICK") {//       ERR: 4, RPL: 0  [OK][OK][OK][OK]
		commandNICK(socket);
    } else if (command == "PRIVMSG") {//    ERR: 7, RPL: 1 [OK][KO][KO][KO][KO][KO][KO][OK]  <--- need more info
		commandPRIVMSG(socket);
    } else if (command == "QUIT") {//       There are no ERR / RPL
    	commandQUIT(socket);
    } else if (command == "WHOIS") {//      OK RPL_WHOISCHANNELS [In process]
    	commandWHOIS(socket);
    } else if (command == "JOIN") {//
		commandJOIN(socket);
    } else if (command == "AWAY") {
    	commandAWAY(socket);
    } else if (command == "ISON") {
        commandISON(socket);
    } else if (command == "LIST") {
        commandLIST(socket); // <----- Need Check
    } else if (command == "PING") {
        commandPING(socket);
    } else if (command == "WHO"){
    	commandWHO(socket);
    } else if (command == "INVITE") {
        commandINVITE(socket); // <--- Nothing
    } else if (command == "PART") {
        commandPART(socket); // <----- Done
    } else if (command == "TOPIC") {//      ERR: 3 [OK][OK][OK], RPL: 2 [OK][OK]
		commandTOPIC(socket);
	}

    socket._msg_buff.clear();
}