#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>

#define COMMAND_COUNT 48

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
        void        stringParser ( std::string &str );
        std::string returnCommand ( std::string &str );
        int         checkCommand ( std::string &str );
        void        checkUSERparam ( std::string &str );
        int         countParam ( std::string &str );
        void        checkNICKparam ( std::string &str );
        





        // void    testList ();




};



#endif