#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include "vector"
#include "cstdio"
#include "ClientSocket.hpp"
#include "User.hpp"

#define COMMAND_COUNT 48
#define SERVER_NAME "127.0.0.1"
#define CODE_TO_STRING(X) #X

class UserDeleteException;

class Parser {
    private:
        std::string _commandList[COMMAND_COUNT];

    public:
        // Con / De - structor
        Parser  ();
        ~Parser ();

        // Copy constructor
        Parser  ( const Parser &other );

        // Assignation operator
		Parser  &operator = ( const Parser &other );

        // External func's
        void                        stringParser ( ClientSocket &socket);
        std::string					returnCommand ( std::string &str );
        bool                        checkCommand ( std::string &str );
        void errSendMsg(int er_code, User& user, const char *msg);

        void                        commandUSER (ClientSocket &socket);
        void                        commandNICK (ClientSocket &socket);
        void                        commandRIVMSG (ClientSocket &socket);
        void                        commandQUIT (ClientSocket &socket);
        void						commandWHOIS(ClientSocket& socket);

        int                         countParam ( std::string &str );
        // int                         countParamSP ( std::string &str );

        std::vector<std::string>    mySplit ( std::string &str );
        // void    testList ();
};



#endif