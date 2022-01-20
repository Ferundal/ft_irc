#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include "vector"
#include "ClientSocket.hpp"
#include "User.hpp"

#define COMMAND_COUNT 48
#define SERVER_NAME "127.0.0.1"

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
        void                        stringParser ( ClientSocket &str );
        std::string                 returnCommand ( std::string &str );
        bool                        checkCommand ( std::string &str );
        void                        workWithUSER ( ClientSocket &str );
        int                         countParam ( std::string &str );
        void                        workWithNICK ( ClientSocket &str );
        std::vector<std::string>    mySplit ( std::string &str );


        // void    testList ();
};



#endif