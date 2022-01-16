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
        void    stringParser ( std::string &str );
        int     checkCommand ( std::string command );

        // void    testList ();




};



#endif