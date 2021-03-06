#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
#include <iostream>
#include "vector"
#include "cstdio"
#include "cstdlib"
#include "ClientSocket.hpp"
#include "User.hpp"
#include "error_answers.hpp"
#include "reply_answers.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define COMMAND_COUNT 48
#define SERVER_NAME "127.0.0.1"
#define TO_STRING_100(X) #X
#define CODE_TO_STRING(X) TO_STRING_100(X)

extern const char* GLOBAL_PASS;

using std::cout;
using std::vector;
using std::string;

struct ClientSocket;
class User;

class Parser {
    private:
        std::string _commandList[COMMAND_COUNT];

    public:
	class UserDeleteException : public std::exception
		{
			const char * what() const throw(){return NULL;};
		};
	public:
        // Con / De - structor
        Parser  ();
        ~Parser ();

        // Copy constructor
        Parser  ( const Parser &other );

        // Assignation operator
		Parser  &operator = ( const Parser &other );

        // External func's
        void                        stringParser (ClientSocket &socket);
        std::string					returnCommand (std::string &str);
        bool                        checkCommand (std::string &str);
		bool						checkModeFlags(std::string &flags);
        void						errSendMsg (const char* er_code, User& user, const char* msg);

        void 						rplSendMsg(const char* rpl_code, User& user, const char* msg);
		void						sendMsgToGroup(const string &sender, const char* rpl_code, const char *group, const vector<User *>& users, const char* msg);

		void						commandKICK (ClientSocket &socket);
        void                        commandUSER (ClientSocket &socket);
        void                        commandNICK (ClientSocket &socket);
        void                        commandPRIVMSG (ClientSocket &socket);
        void                        commandQUIT (ClientSocket &socket);
        void						commandWHOIS (ClientSocket& socket);
        void 						commandJOIN (ClientSocket& socket);
        void 						commandAWAY (ClientSocket& socket);
        void 						commandISON (ClientSocket& socket);
        void 						commandLIST (ClientSocket& socket);
        void 						commandPING (ClientSocket& socket);
		void 						commandWHO (ClientSocket& socket);
        void 						commandINVITE (ClientSocket& socket);
        void 						commandPART (ClientSocket& socket);
		void 						commandTOPIC (ClientSocket& socket);
		void						commandPASS (ClientSocket& socket);
      
        int                         countParam (std::string &str);
		void						commandMODE (ClientSocket& socket);
		void						commandNOTICE (ClientSocket& socket);


        std::vector<std::string>    mySplit ( std::string str );
};



#endif