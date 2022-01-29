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
	(void) er_code;
	std::string message;
	message = message + ":" + SERVER_NAME + " " + er_code + " " + user.GetUserNick() + " " + msg + "\r\n";
	write(1, message.data(), message.size()); // DEBUG out
	send(user.GetUserFd(), message.data(), message.size(), 0);
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


// int          Parser::countParam ( std::string &str ) {
//     int     slovo, count = 0;
//     size_t  i = 0;
//     int     flag = 0;
//     char    symb = '\r';

//     while (i < str.length()) {
//         if (str[i] == ':') {
//             flag = 1;
//             symb = ':';
//         }
//         ++i;
//     }

//     i = 0;

//     while (str[i] == ' ' && str[i] != symb)
//         i++;
//     slovo = 0;

//     while (str[i] != symb) {
//         if (str[i] != ' ' && slovo == 0) {
//             slovo = 1;
//             count++;
//         } else if (str[i] == ' ')
//             slovo = 0;
//         i++;
//     }

//     return count + flag;
// }

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

// std::vector<std::string> Parser::mySplit ( std::string str ) {
//     size_t      i = 0;
//     std::string symb = " ";
//     size_t      countP = countParam(str);
//     std::vector<std::string> commandArr(countP);

// //    std::cout << "Count" << countP << std::endl;
//     while (i < countP) {
//         if (i == countP - 1)
//             symb = "\r";
//         commandArr[i] = str.substr(0, str.find(symb));
// //        std::cout << "commandArr[" << i << "]: " << commandArr[i] << std::endl;
//         str.erase(0, str.find(symb) + 1);
//         ++i;
//     }
//     cout << "SplitParamCount: " << countP<< endl;

//     return commandArr;
// }

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
//  Checking repeat NICK and USER in DB
    bool                        isSetUserInfo;
    std::vector<std::string>    paramList;
    std::string                 answer;


    paramList = mySplit(socket._msg_buff);

    // Check ERR_NEEDMOREPARAMS
    if (paramList.size() != 5) {
        errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr, (paramList[0] + " :Not enough parameters").data());
        return;
    }

    // Check ERR_ALREADYREGISTRED

    if (socket._usr_ptr->IsUserInfoSet() == true) {
        errSendMsg(CODE_TO_STRING(ERR_ALREADYREGISTRED), *socket._usr_ptr, (paramList[0] + " :You may not reregister").data());
        return;
    }

	if (socket._usr_ptr->IsActive() == false) {
        isSetUserInfo = socket._usr_ptr->SetUserInfo(paramList[1], paramList[2], paramList[3], paramList[4]);
        if (isSetUserInfo == false) {
            if (socket._usr_ptr->SetActive() == 0) {
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_MOTDSTART) + " " + socket._usr_ptr->GetUserFullName()
						 + " :- " + SERVER_NAME + " Message of the day -\r\n";
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_MOTD) + " " + socket._usr_ptr->GetUserFullName()
						 + " :- " + SERVER_NAME + " Welcome to the party\r\n";
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_ENDOFMOTD) + " " + socket._usr_ptr->GetUserFullName()
						 + " :" + SERVER_NAME + "End of /MOTD command\r\n";
                std::cout << "Answer " << answer << std::endl;
                send(socket._fd, answer.data(), answer.size(), 0);
            }
        }
    }
}

void        Parser::commandNICK (ClientSocket &socket ) {
    //  Checking repeat NICK and USER in DB
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
        errSendMsg(CODE_TO_STRING(ERR_NONICKNAMEGIVEN), *socket._usr_ptr, ":No nickname given");
        return;
    }

    // Check ERR_ERRONEUSNICKNAME
    string s_ascii_err = "!\"#$%&\'()*+`-./:;<=>?@^_{}|~";
    for (size_t i = 0; i < paramList[1].size(); ++i)
    {
        if (s_ascii_err.find(paramList[1][i]) !=  string::npos)
            {
                errSendMsg(CODE_TO_STRING(ERR_ERRONEUSNICKNAME), *socket._usr_ptr, (paramList[1] + " :Erroneus nickname").data());
                return;
            }
    }


    // Check ERR_NICKNAMEINUSE
    if (paramList.size() == 3) {
        if (socket._usr_ptr->SetNick(paramList[2]) != false) {
            errSendMsg(CODE_TO_STRING(ERR_NICKNAMEINUSE), *socket._usr_ptr, (paramList[2] + " :Nickname is already in use").data());
            return;
        }
    }



	if (socket._usr_ptr->IsActive() == false) {
        isSetNickInfo = socket._usr_ptr->SetNick(paramList[1]);
        if (isSetNickInfo == false) {
            // std::cout << "|INFO| [Nick successfuly added]" << std::endl;
            if (socket._usr_ptr->SetActive() == 0) {
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_MOTDSTART) + " " + socket._usr_ptr->GetUserNick() //TODO поменять на дефайн
						 + " :- " + SERVER_NAME + " Message of the day -\r\n";
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_MOTD) + " " + socket._usr_ptr->GetUserNick() //TODO поменять на дефайн
						 + " :- " + SERVER_NAME + " Middle request\r\n";
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_ENDOFMOTD) + " " + socket._usr_ptr->GetUserNick() //TODO поменять на дефайн
						 + " :" + SERVER_NAME + "End of /MOTD command\r\n";
                std::cout << answer << std::endl;
                send(socket._fd, answer.data(), answer.size(), 0);
            }
        }
    }
}

void    Parser::commandPRIVMSG (ClientSocket &socket ) {

    std::vector<std::string>    paramList = mySplit(socket._msg_buff);
    std::string command = paramList[0];
	int param_count = countParam(socket._msg_buff);
	if (param_count < 3)
	{
		errSendMsg(CODE_TO_STRING(ERR_NORECIPIENT),*socket._usr_ptr, (":No recipient given ("+ command+ ")").data());
		return;
	}

    // TODO реализовать отправку по всем никнеймам?
    std::string nick = paramList[1];
    std::string message;
    User        *receiver = socket._usr_ptr->ToStore().FindUserByNick(nick);

    if (receiver == NULL) {
    	errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr, (nick+" :No such nick/channel").data());
    	return;
    }

	message	= ":" + socket._usr_ptr->GetUserNick() + " PRIVMSG " + nick + " " + paramList[2] + "\r\n";
    cout << message << endl;
	send(receiver->GetUserFd(), message.data(), message.size(), 0);

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

void	Parser::commandWHOIS(ClientSocket& socket)
{
	int param_count = countParam(socket._msg_buff);
	if(param_count != 2)
	{
		errSendMsg(CODE_TO_STRING(ERR_NONICKNAMEGIVEN), *socket._usr_ptr,":No nickname given");
		return;
	}

	string message;
	std::vector<std::string>	paramList = mySplit(socket._msg_buff);
	std::string  nick = paramList[1];
	User* user; // По кому ищется и отправляется инфа

	if((user = socket._usr_ptr->ToStore().FindUserByNick(paramList[1])) == NULL)
	{
		errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr, (nick + " :No such nick/channel").data());
		return;
	}
	else{
		message = message + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_WHOISUSER) + " " + socket._usr_ptr->GetUserNick() + " " +
				user->GetUserNick() + " " + user->GetUserName() + " " + user->GetUserHost() + " * " +
				user->GetUserRealName() + "\r\n";
	}

	message = message + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_ENDOFWHOIS) + " " + socket._usr_ptr->GetUserNick()
			+ " " + user->GetUserNick() + " :End of /WHOIS list\r\n";
	cout << message << endl;
	send(socket._fd, message.data(), message.size(), 0);
//ERR_NONICKNAMEGIVEN(Ok)					  RPL_WHOISSERVER
//RPL_WHOISUSER(Ok)                           RPL_WHOISCHANNELS??
//RPL_AWAY(кто отошел)                        RPL_WHOISOPERATOR
//RPL_WHOISIDLE(Кто простаивает)              ERR_NOSUCHNICK(Ok)
//RPL_ENDOFWHOIS(Ok)
}

void	Parser::commandJOIN(ClientSocket& socket)
{
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
	std::string command = paramList[0];
	int param_count = countParam(socket._msg_buff);

	if(param_count != 2)
	{
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr,(command + " :Not enough parameters").data());
		return;
	}
	std::cout << ">>" << paramList[1] << "<<" << std::endl;
	socket._usr_ptr->JoinChannel(paramList[1], ""); // TODO сделать норм

	string message;

	message = message + ":" + socket._usr_ptr->GetUserNick() + " JOIN " + paramList[1] + "\r\n";
//
	message = message + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_TOPIC) + " " + socket._usr_ptr->GetUserNick() + " "
			+ paramList[1] + " :TOPIC\r\n";

//	353     RPL_NAMREPLY
//	"<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
	message = message + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_NAMREPLY) + " " + socket._usr_ptr->GetUserNick() + " "
			+ paramList[1] + " :@archie " + socket._usr_ptr->GetUserNick() + "\r\n"; // TODO добавить список ников

//	366     RPL_ENDOFNAMES
//	"<channel> :End of /NAMES list"
	message = message + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_ENDOFNAMES) + " " + socket._usr_ptr->GetUserNick() + " "
			+ paramList[1] + " :End of /NAMES list\r\n";

	cout << message << endl;
	send(socket._fd, message.data(), message.size(), 0);
//	ERR_NEEDMOREPARAMS(Ok)          ERR_BANNEDFROMCHAN
//	ERR_INVITEONLYCHAN              ERR_BADCHANNELKEY
//	ERR_CHANNELISFULL               ERR_BADCHANMASK
//	ERR_NOSUCHCHANNEL               ERR_TOOMANYCHANNELS
//	RPL_TOPIC
//	RPL_NAMREPLY
}


void 						Parser::commandAWAY( ClientSocket& socket ) {
//      Команда: AWAY
//   Параметры: [message]

//   С сообщением AWAY, клиенты могут устанавливать автоматическую строку
//   ответа на любые PRIVMSG-команды, направленные им (не на канал).
//   Автоматический ответ посылается сервером к клиенту, пославшего команду
//   PRIVMSG. Только отвечающий сервер может быть только один, к которому
//   подсоединен клиент.

//   AWAY используется вместе с одним параметром (установка сообщения AWAY)
//   или без параметров (снятие сообщения AWAY).

//   Числовые ответы:

//           RPL_UNAWAY                      RPL_NOWAWAY

    std::string                 answer;
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);

    if (paramList.size() == 1) {
        answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_UNAWAY) + " " + socket._usr_ptr->GetUserNick() + ":You are no longer marked as being away\r\n";
        std::cout << answer << std::endl;
        send(socket._fd, answer.data(), answer.size(), 0);
        socket._usr_ptr->SetNotAway();
    } else {
        answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_NOWAWAY) + " " + socket._usr_ptr->GetUserNick() + ":You have been marked as being away\r\n";        
        std::cout << answer << std::endl;
        send(socket._fd, answer.data(), answer.size(), 0);
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
        std::cout << answer << std::endl;
        send(socket._fd, answer.data(), answer.size(), 0);
    } else {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr, (nicknameList[0] + " :Not enough parameters").data());
        return;
    }
}




void 						Parser::commandLIST (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
	list<std::string>           listOfChanels;
    std::string                 answer;


    answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_LISTSTART) + " " + socket._usr_ptr->GetUserNick() + " Channel :Users  Name\r\n";
    std::cout << answer << std::endl;
    send(socket._fd, answer.data(), answer.size(), 0);

    if (paramList.size() == 1) {
        listOfChanels = socket._usr_ptr->ChannelList("");
        list<std::string>::iterator currChanel= listOfChanels.begin();
        list<std::string>::iterator chanelsEnd= listOfChanels.end();
        while (currChanel != chanelsEnd) {
            answer = "";
            answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_LIST) + " " + socket._usr_ptr->GetUserNick() + " " + *currChanel + "\r\n";
            send(socket._fd, answer.data(), answer.size(), 0);
            std::cout << answer << std::endl;
            ++currChanel;
        }
    } else {
        for (size_t i = 0; i < paramList.size(); ++i) {
            listOfChanels = socket._usr_ptr->ChannelList(paramList[i]);
            if (!listOfChanels.empty()) {
                answer = "";
                answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_LIST) + " " + socket._usr_ptr->GetUserNick() + " " + *listOfChanels.begin() + "\r\n";
                send(socket._fd, answer.data(), answer.size(), 0);
                std::cout << answer << std::endl;
            }
        }
    }
    answer = "";
    answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_LISTEND) + " " + socket._usr_ptr->GetUserNick() + " :End of /LIST\r\n";
    send(socket._fd, answer.data(), answer.size(), 0);
    std::cout << answer << std::endl;

}


void 						Parser::commandPING (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
    std::cout << "PONG " << paramList[1] << std::endl;

    // std::cout << << std::endl;
}

void 						Parser::commandWHO(ClientSocket& socket)
{
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
	std::string                 command;
	std::string                 answer;

	socket._usr_ptr->SetUserInfo(paramList[1], paramList[2], paramList[3], paramList[4]);
	answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_WHOREPLY) +
			" " + paramList[1] + " has 4 users. Operator: psina\r\n";
	answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_ENDOFWHO) +
			" * :End of /WHO list\r\n";
	cout << answer << endl;
	send(socket._fd, answer.data(), answer.size(), 0);
//	USER NePess * 127.0.0.1 :Shuchu Pes
//	352     RPL_WHOREPLY
//	"<channel> <user> <host> <server> <nick> \
//  <H|G>[*][@|+] :<hopcount> <real name>"
//
//  315     RPL_ENDOFWHO
//  "<name> :End of /WHO list"
//
//  ERR_NOSUCHSERVER

}

void 						Parser::commandINVITE (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);
    std::string                 answer;
    User*						reciever = socket._usr_ptr->ToStore().FindUserByNick(paramList[1]);

    // Checking NEEDMOREPARAM
    if (paramList.size() != 3) {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr, (paramList[0] + " :Not enough parameters").data());
        return;
    }

    // Checking ERR_NOSUCHNICK
    if (reciever == NULL) {
    	errSendMsg(CODE_TO_STRING(ERR_NOSUCHNICK), *socket._usr_ptr, (paramList[1] + " :No such nick/channel").data());
        return;
    }

    // Checking ERR_NOTONCHANNEL
    if (socket._usr_ptr->LeaveChannel(paramList[2]) == 442) {
        errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr, (paramList[2] + " :You're not on that channel").data());
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

//	reciever->IsAway()

    answer = answer + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_INVITING) + " " + paramList[2] + " " + paramList[1] + "\r\n";
	send(socket._fd, answer.data(), answer.size(), 0);
	cout << answer << endl;
	answer.clear();
    answer = answer + ":" + socket._usr_ptr->GetUserNick() + " INVITE " + paramList[1] + " " + paramList[2] + "\r\n";
    cout << answer << endl;
    send(reciever->GetUserFd(), answer.data(), answer.size(), 0);

    //  :Angel INVITE Wiz #Dust         ; Пользователь Angel пригласил WiZ на
    //                                  канал #Dust
}

void 						Parser::commandPART (ClientSocket& socket) {
	std::vector<std::string>    paramList = mySplit(socket._msg_buff);

    // [CHECK] There are no same Chanels in ListOfChanels
    for (size_t i = 1; i < paramList.size(); ++i) {
        if ((socket._usr_ptr->ToStore().FindChannelByName(paramList[i]) == NULL)) {
		    errSendMsg(CODE_TO_STRING(ERR_NOSUCHCHANNEL), *socket._usr_ptr, (paramList[i] + " :No such channel").data());
            return;
        }
    }
    
    // [CHECK] There are more than 1 parameters in ListOfParameters
    if (paramList.size() >= 2) {
        for (size_t i = 1; i < paramList.size(); ++i) {
            if(socket._usr_ptr->LeaveChannel(paramList[i]) == 442) {
                std::cout << "[PART]: User was deleted" << std::endl;
    // [CHECK] There is not on chanel
        		errSendMsg(CODE_TO_STRING(ERR_NOTONCHANNEL), *socket._usr_ptr, (paramList[i] + " :You're not on that channel").data());
                return;
            }
        }
    } else {
		errSendMsg(CODE_TO_STRING(ERR_NEEDMOREPARAMS), *socket._usr_ptr, (paramList[0] + " :Not enough parameters").data());
        return;
    }
}


// PING
// WHO
// INVITE
// LIST
// PART
void    Parser::stringParser(ClientSocket &socket) {
    std::cout << socket._msg_buff << std::endl; //DEBUG out
    socket._msg_buff.erase(socket._msg_buff.size() - 1, 1);

    std::string command = returnCommand(socket._msg_buff);

    if (!checkCommand(command))
    {
    	errSendMsg(CODE_TO_STRING(ERR_UNKNOWNCOMMAND), *socket._usr_ptr, (command + " :Unknown command").data());
    	socket._msg_buff.clear();
    	return;
    }

    if (command == "USER"){
		commandUSER(socket);
    } else if (command == "NICK") {
		commandNICK(socket);
    } else if (command == "PRIVMSG") {
		commandPRIVMSG(socket);
    } else if (command == "QUIT") {
    	commandQUIT(socket);
    } else if (command == "WHOIS") {
    	commandWHOIS(socket);
    } else if (command == "JOIN"){
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
    }

    socket._msg_buff.clear();
}