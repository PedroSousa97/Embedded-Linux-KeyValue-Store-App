#include <unistd.h>
#include <iostream>
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include "ClientOBJ.h"
#include "Utility.h"

using namespace std;

//Simple constructor that sets socket port and server point=NULL for now
Client::Client()
{
    this->portno = 8080;
    this->server = NULL;
}

//Method that opens the socket
eErrorType Client::OpenSocket()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;
    //open socket
    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd < 0)
    {
        Error = SOCKETOPENFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}

//Method that gets the hostname
eErrorType Client::GetHost(std::string hostname)
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;
    //Get hostname, save it to server pointer
    this->server = gethostbyname(hostname.c_str());
    if (this->server == NULL)
    {
        Error = GETHOSTFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}

//Method that connects the client to the server
eErrorType Client::ClientConnect()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;

    //clear address structure
    bzero((char *)&this->serv_addr, sizeof(this->serv_addr));
    //Set address parameters
    this->serv_addr.sin_family = AF_INET;
    bcopy((char *)this->server->h_addr, (char *)&this->serv_addr.sin_addr.s_addr, this->server->h_length);
    this->serv_addr.sin_port = htons(portno);
    //connect to the server
    int conRet = connect(this->sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (conRet < 0)
    {
        Error = CONNECTFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}

//Method used to send commands to the server
eErrorType Client::ClientWrite()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;
    //send message to the server
    int WrtRet = write(this->sockfd, this->buffer, strlen(this->buffer));
    if (WrtRet < 0)
    {
        Error = WRITEFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}
//Method used to read messages sent from the Server
eErrorType Client::ClientRead()
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;
    //Clear Object buffer
    bzero(this->buffer, 256);
    //Read data and save into the object buffer
    int RdRet = read(this->sockfd, this->buffer, 255);
    if (RdRet < 0)
    {
        Error = READFAIL;
        cout << "Error Code :" << errno << endl;
    }
    //If everything went as expected return Success code
    return Error;
}
//Simple method used to close the socket
void Client::CloseSocket()
{
    close(this->sockfd);
}