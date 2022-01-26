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


int          Parser::countParam ( std::string &str ) {
    int     slovo, count = 0;
    size_t  i = 0;
    int     flag = 0;
    char    symb = '\r';

    while (i < str.length()) {
        if (str[i] == ':') {
            flag = 1;
            symb = ':';
        }
        ++i;
    }

    i = 0;

    while (str[i] == ' ' && str[i] != symb)
        i++;
    slovo = 0;

    while (str[i] != symb) {
        if (str[i] != ' ' && slovo == 0) {
            slovo = 1;
            count++;
        } else if (str[i] == ' ')
            slovo = 0;
        i++;
    }

    return count + flag;
}

std::vector<std::string> Parser::mySplit ( std::string str ) {
    size_t      i = 0;
    std::string symb = " ";
    size_t      countP = countParam(str);
    cout << "SplitParamCount: " << countP<< endl;
    std::vector<std::string> commandArr(countP);

//    std::cout << "Count" << countP << std::endl;
    while (i < countP) {
        if (i == countP - 1)
            symb = "\r";
        commandArr[i] = str.substr(0, str.find(symb));
//        std::cout << "commandArr[" << i << "]: " << commandArr[i] << std::endl;
        str.erase(0, str.find(symb) + 1);
        ++i;
    }
    cout << "SplitParamCount: " << countP<< endl;

    return commandArr;
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

void    Parser::commandRIVMSG (ClientSocket &socket ) {

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
	cout << "QUIT command done" << endl; // DEBUG out
	socket._usr_ptr->ToStore().DeleteUser(socket._usr_ptr);
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

	string message;

	message = message + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_TOPIC) + " " + socket._usr_ptr->GetUserNick() + " "
			+ paramList[1] + " :TOPIC\r\n";

//	353     RPL_NAMREPLY
//	"<channel> :[[@|+]<nick> [[@|+]<nick> [...]]]"
//	message = message + ":" + SERVER_NAME + " " + CODE_TO_STRING(RPL_NAMREPLY) + " " //+ socket._usr_ptr->GetUserNick() + " "
//			+ paramList[1] + " :@archie0 kreker" + "\r\n"; // TODO добавить список ников

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


// AWAY :Прямо сейчас меня здесь нет  | AWAY :Отошел    -- когда отошел
// AWAY									-- когда пришел
// ISON <nick> <nick> ...   --- запрашивает ники тех, кто доступен



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

    answer = "AWAY :" + paramList[1];

}

void    Parser::stringParser(ClientSocket &socket) {
    std::cout << socket._msg_buff << std::endl; //DEBUG out
    socket._msg_buff.erase(socket._msg_buff.size() - 1, 1);

    std::string command = returnCommand(socket._msg_buff);


    // std::cout << "COmmand: " << command << std::endl; //DEBUG out


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
		commandRIVMSG(socket);
    } else if (command == "QUIT") {
    	commandQUIT(socket);
    } else if (command == "WHOIS") {
    	commandWHOIS(socket);
    } else if (command == "JOIN"){
		commandJOIN(socket);
    } else if (command == "AWAY") {
    	commandAWAY(socket);
    }

    socket._msg_buff.clear();
}