#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <strings.h>
#include <errno.h>
#include "ServerOBJ.h"

using namespace std;

//Class constructor, simply print a creation message
Server::Server()
{
    cout << GREEN << "Server object created" << CNORMAL << endl;
}

//Open Socket method, it returns one of the Enum Error Types
eErrorType Server::OpenSocket()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;

    //create socket
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd < 0)
    {
        Error = SOCKETOPENFAIL;
        cout << "Error Code :" << errno << endl;
        return Error;
    }
    //SET SO_REUSEADDR socket option, this makes it possible to unbind and bind socket without waiting for TCP TIME_WAIT
    int reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuse, sizeof(reuse)) < 0)
        cerr << "setsockopt(SO_REUSEADDR) failed" << endl;

    //clear address structure
    bzero((char *)&this->serv_addr, sizeof(this->serv_addr));

    //Communication port
    this->portno = 8080;
    //Set address parameters
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; //For this example bind to any interface
    serv_addr.sin_port = htons(portno);     //Set communication port from host byte order to network byte order.

    //If everything went as expected return Success code
    return Error;
}

//Bind Server method
eErrorType Server::BindServer()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;
    //Bind socket
    int bindRet = bind(sockfd, (struct sockaddr *)&this->serv_addr, sizeof(this->serv_addr));
    if (bindRet < 0)
    {
        Error = BINDFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}

// Server listen method
eErrorType Server::ListenServer()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;

    //listen for a cliente, in this case only 1 cliente, doesn't make sense to have a queue
    int lisret = listen(sockfd, 1);
    if (lisret < 0)
    {
        Error = LISTENFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}

//Accept client method
eErrorType Server::AcceptServer()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;
    //Accept Client and store socket in newsockfd variable
    this->newsockfd = accept(this->sockfd, (struct sockaddr *)&this->cli_addr, &this->clientlen);
    if (this->newsockfd < 0)
    {
        Error = ACCEPTFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}

// Method used to send messages to the client
eErrorType Server::SendServer(std::string msg)
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;
    //Send a string transformed into char through the socket to the client
    ssize_t sndRet = send(this->newsockfd, msg.c_str(), msg.length(), 0);
    if (sndRet < 0)
    {
        Error = SENDFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}

//Method that receives data from the client and stores it in the object buffer
eErrorType Server::ReceiveServer()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;

    //clear object buffer from previous messages
    bzero(buffer, 256);

    //Save the message to the object buffer
    ssize_t rdRet = read(this->newsockfd, buffer, 256);
    if (rdRet < 0)
    {
        Error = RECEIVEFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}
//Simple method to close the socket
void Server::CloseSocket()
{
    close(this->sockfd);
}
