#ifndef __CLIENTOBJ_H__
#define __CLIENTOBJ_H__

#ifndef Client_H
#define Client_H
#include <netinet/in.h>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include "Utility.h"

//CONSOLE OUTPUT COLORS
#define CNORMAL "\x1B[0m"   //Default color
#define LIGHTRED "\x1B[91m" //Red - for DB operations error messages
#define GREEN "\x1B[32m"    //Green - for DB operations success messages

/*
*   Client class declaration in header file
*   Wrapping the socket logic as a client object 
*   This makes the code more understandable and easy to use
*/
class Client
{
public:
    //variables to hold the socket and port number
    int sockfd, portno;
    //socket address structure that will hold the server address information
    struct sockaddr_in serv_addr;
    //socket address pointer, used to get hostname and consequent server address
    struct hostent *server;
    //Client message buffer, 256 byte should be more than enough, and it's a good convention
    char buffer[256];
    //Class constructor
    Client();
    //Method to open a socket
    eErrorType OpenSocket();
    //Method used to optain hostname
    eErrorType GetHost(std::string hostname);
    //Method to connect client to socket
    eErrorType ClientConnect();
    //Method used to write/send commands to the server
    eErrorType ClientWrite();
    //Method used to read data from the server
    eErrorType ClientRead();
    //Method used to close the socket
    void CloseSocket();
};

#endif

#endif //  __CLIENTOBJ_H__