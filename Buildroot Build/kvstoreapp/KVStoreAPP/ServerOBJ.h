#ifndef __SERVEROBJ_H__
#define __SERVEROBJ_H__

#include <netinet/in.h>
#include <sys/socket.h>
#include "Utility.h"

/*
*   Server class declaration in header file
*   Wrapping the socket logic as a Server object 
*   This makes the code more understandable and easy to use
*/

//CONSOLE OUTPUT COLORS
#define CNORMAL "\x1B[0m"   //Default color
#define LIGHTRED "\x1B[91m" //Red - for DB operations error messages
#define GREEN "\x1B[32m"    //Green - for DB operations success messages

class Server
{
public:
    //variables to hold the socket and port number
    int sockfd, newsockfd, portno;
    //socklen variable used to hold the client length when accepting a client
    socklen_t clientlen;

    //Server message buffer, 256 byte should be more than enough, and it's a good convention
    char buffer[256];

    //socket address structure that will hold the client and server address information
    struct sockaddr_in serv_addr, cli_addr;

    Server();
    //Method to open a socket
    eErrorType OpenSocket();
    //Method to bind the socket to a specific address
    eErrorType BindServer();
    //Method to listen for clients
    eErrorType ListenServer();
    //Method that accepts a cliente
    eErrorType AcceptServer();
    //Method used to send a message to the Client
    eErrorType SendServer(std::string msg);
    //Method used to receive data from the client
    eErrorType ReceiveServer();
    //Method used to close the socket
    void CloseSocket();
};

#endif //  __SERVEROBJ_H__
