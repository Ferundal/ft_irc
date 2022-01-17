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


/**
 *                       !!! __FOR_VOVA__ !!! 
 *
 * Algorithm doesn't work with (symbol) '\n' on the end of the string 
 */


std::string Parser::returnCommand ( std::string &str ) {
    std::string command;
    int         pos;

    pos = str.find(' ');
    if (pos < 0)
        return (str);
    else
        return (str.substr(0, pos));
}

int         Parser::checkCommand ( std::string &str ) {
    std::string command;

    command = returnCommand(str);
    for (int i = 0; i < COMMAND_COUNT; ++i)
        if (this->_commandList[i] == command) {
            return 0;
        }
    return 1;
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
    std::cout << "COUT: " << count << std::endl;
    return count;
}

// USER archie * 127.0.0.1 :purple

void         Parser::checkUSERparam ( std::string &str ) {
    if (countParam(str) != 5) {
        std::cout << "ERROR: INVALID NUM OF PARAM IN USER" << std::endl;
    } else {
        std::cout << "RESULT: USER NUM OF PARAM IS OK" << std::endl;
    }

}

void         Parser::checkNICKparam ( std::string &str ) {
    if (countParam(str) > 1 && countParam(str) <= 3) {
        std::cout << "RESULT: USER NUM OF PARAM IS OK" << std::endl;
    } else {
        std::cout << "ERROR: INVALID NUM OF PARAM IN USER" << std::endl;
    }

}


void        Parser::stringParser ( std::string &str ) {

    if (checkCommand(str) != 0) {
        std::cout << "ERROR: INVALID COMMAND" << std::endl;
        // std::cout << "" << std::endl;
    } else {
        std::cout << "RESULT: VALID COMMAND" << std::endl;
        // std::cout << "" << std::endl;
    }



    if (returnCommand(str) == "USER") {
        checkUSERparam(str);
    } else if (returnCommand(str) == "NICK") {
        checkNICKparam(str);
    }



}




