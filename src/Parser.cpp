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


std::string Parser::returnCommand ( std::string &str ) {
    std::string command;
    int         pos;

    pos = str.find(' ');
    if (pos < 0)
        return (str);
    else
        return (str.substr(0, pos));
}

bool         Parser::checkCommand ( std::string &str ) {
    std::string command;

    command = returnCommand(str);
    for (int i = 0; i < COMMAND_COUNT; ++i)
        if (this->_commandList[i] == command) {
            return true;
        }
    return false;
}

int          Parser::countParam ( std::string &str ) {
    int     slovo, count = 0;
    int     i = 0;
    while (str[i] == ' ' && str[i] != '\0')
        i++;
    slovo = 0;
    while (str[i] != '\0') {
        if (str[i] != ' ' && slovo == 0) {
            slovo = 1;
            count++;
        } else if (str[i] == ' ')
            slovo = 0;
        i++;
    }
    // std::cout << "COUT: " << count << std::endl;
    return count;
}

std::vector<std::string> Parser::mySplit ( std::string &str ) {

    size_t      i = 0;
    size_t      countP = countParam(str);
    std::vector<std::string> commandArr(countP);

    while (i < countP) {
        commandArr[i] = str.substr(0, str.find(" "));
        str.erase(0, str.find(" ") + 1);
        ++i;
    }

    return commandArr;
}



void         Parser::workWithUSER ( ClientSocket &str ) {
//  Checking repeat NICK and USER in DB
    bool                        isActive;
    bool                        isSetUserInfo;
    std::vector<std::string>    paramList;

    paramList = mySplit(str._msg_buff);


    isActive = str._usr_ptr->IsActive();
    ;// std::cout << "|INFO| isActive: [" << isActive << "]" << std::endl;
    if (isActive == false) {
        isSetUserInfo = str._usr_ptr->SetUserInfo(paramList[1], paramList[2], paramList[3], paramList[4]);
        // ;std::cout << "|INFO| isSetUserInfo: [" << isSetUserInfo << "]" << std::endl;
        if (isSetUserInfo == false) {
            ;// std::cout << "|INFO| [User successfuly added]" << std::endl;
            if (str._usr_ptr->SetActive() == 0) {
                str._msg_buff = "375     RPL_MOTDSTART";
            }
            else
            {
                str._msg_buff.clear();
            }
            // std::cout << "Res: " << str._usr_ptr->SetActive() << std::endl;
        }
        else
           ;// std::cout << "|INFO| [User is not added]" << std::endl;

    } else {
        ;// std::cout << "|INFO| [User is already active]" << std::endl;
    }
}


void        Parser::workWithNICK ( ClientSocket &str ) {
    //  Checking repeat NICK and USER in DB
    bool                        isActive;
    bool                        isSetNickInfo;
    std::vector<std::string>    paramList;

    paramList = mySplit(str._msg_buff);


    isActive = str._usr_ptr->IsActive();
    // std::cout << "|INFO| isActive: [" << isActive << "]" << std::endl;
    if (isActive == false) {
        isSetNickInfo = str._usr_ptr->SetNick(paramList[1]);
        // std::cout << "|INFO| isSetNickInfo: [" << isSetNickInfo << "]" << std::endl;
        if (isSetNickInfo == false) {
            // std::cout << "|INFO| [Nick successfuly added]" << std::endl;
            if (str._usr_ptr->SetActive() == 0) {
                str._msg_buff = 
                ":127.0.0.1 375 KrekerNick :- 127.0.0.1 Message of the day - \n\r\n"
				":127.0.0.1 372 KrekerNick :- HuiHuiHui\n\r\n"
				":127.0.0.1 376 KrekerNick :End of /MOTD command\n\r\n";
                //"001 RPL_WELCOME Welcome to the Internet Relay Network archi_pes!:purple@127.0.0.1";
            }
            else
            {
                str._msg_buff.clear();
            }
            // std::cout << "Res: " << str._usr_ptr->SetActive() << std::endl;
        }
        else
            ;// std::cout << "|INFO| [Nick is not added]" << std::endl;

    } else {
        ;// std::cout << "|INFO| [Nick is already active]" << std::endl;
    }


}

void    Parser::stringParser ( ClientSocket &str ) {

    //  Checking command in pull
    bool result;

    result = checkCommand(str._msg_buff);
    // std::cout << "|NOTE| FIND COMMAND RESULT: [" << result << "]" << std::endl;

    //  Getting command for countin. logic
    std::string command;

    command = returnCommand(str._msg_buff);
    // std::cout << "|NOTE| COMMAND FIND: [" << command << "]" << std::endl;


    if (command == "USER") {
        workWithUSER (str);
    } else if (command == "NICK") {
        workWithNICK (str);   
    }

    // str._msg_buff.clear();
}






















