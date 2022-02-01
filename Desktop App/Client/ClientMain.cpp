#include <iostream>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include "ClientOBJ.h"

using namespace std;

//Simple function used to print errors
void PrintError(std::string errMsg)
{
    cout << errMsg << endl;
}

//Send Command function that sends a command to the server and waits for a response/acknowlegde
void SendCommand(Client *clientObj, std::string cmd)
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;

    //clear object buffer from previous commands
    bzero(clientObj->buffer, 256);
    //Copy input command to the buffer
    strcpy(clientObj->buffer, cmd.c_str());
    //send command to the server
    Error = clientObj->ClientWrite();
    if (Error != SUCCESS)
    {
        PrintError("Unable to Write to server from client");
    }
    //If the command was to close the app (EXIT_IPC), don't wait for acknowledge
    if (cmd != "(EXIT_IPC)")
    {
        //acknowlegde received/ message from client
        Error = clientObj->ClientRead();
        if (Error != SUCCESS)
        {
            PrintError("Client Unable to read from server");
        }
        //Print message received from server
        cout << "Message Received From server: " << clientObj->buffer << endl;
    }
}

int main()
{
    cout << "******************************* KVStoreApp IPC ********************************" << endl;
    cout << "Your are currently connected to the KVStoreAPP using IPC." << endl;
    cout << "It is a persistent in disk Key-Value pair storage console application." << endl;
    cout << "Having that into account you got the following request available:" << endl;
    cout << "SET [key] [value] -> Store a new Key-Value pair to the disk; " << endl;
    cout << "GET [key] [value] -> GET the value of a secific key, if it exists; " << endl;
    cout << "DELETE [key] [value] -> DELETE Key-Value pair from storage, with specified key;" << endl
         << endl;
    cout << "Requests Commands other than the ones specified will be ingored, as well as " << endl;
    cout << "command parameters additional to the ones expected." << endl
         << endl;
    cout << "Other than the commands above, you can also terminate this IPC connection" << endl;
    cout << "Use the (EXIT_IPC) command, including parenthesis when you are ready to leave " << endl;
    cout << "Have fun! That's what coding and computer interaction is all about! :)" << endl;
    cout << "*******************************************************************************" << endl;

    //instantiate the Client
    Client clientObj;

    //Prepare buffer that will hold the hostname
    char hostbuffer[256];

    //Begin with success code and change later on depending on the result of the requests
    //and socket operations
    eErrorType Error = SUCCESS;

    // Prepare the buffer for the line the user enters
    std::string cinRequest;

    //open socket
    Error = clientObj.OpenSocket();
    if (Error != SUCCESS)
    {
        PrintError("Client Open Socket Fail");
        return 0;
    }

    //clear hostname buffer
    bzero(hostbuffer, 256);
    //get hostname
    int hostnameRet = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostnameRet < 0)
    {
        PrintError("Unable to local machine get Host Name");
        return 0;
    }
    //Get hostname into the object
    Error = clientObj.GetHost(hostbuffer);
    if (Error != SUCCESS)
    {
        PrintError("Unable to get Host from client");
        return 0;
    }

    //Connect Client
    Error = clientObj.ClientConnect();
    if (Error != SUCCESS)
    {
        PrintError("Unable to Connect to server");
        return 0;
    }

    int isExit = 1;

    while (isExit)
    {
        //Get input from user that is currently using the client interface
        std::getline(cin, cinRequest);
        //If he didn't input anything ignore and request new input
        if (cinRequest.empty())
        {
            cout << "Sorry, but I think " << LIGHTRED << "you didn't input anything." << endl;
            cout << CNORMAL << "Please try once again :)" << endl;
        }
        else
        {
            //If the user indeed input something, then send it to the server
            SendCommand(&clientObj, cinRequest);
            //If the input is (EXIT_IPC) Then close the socket and apllication
            if (cinRequest == "(EXIT_IPC)")
            {
                cout << "Hope you enjoyed it :) Closing Connection and Application. See you next time!" << endl;

                //Close socket
                clientObj.CloseSocket();
                //Close application
                return 0;
            }
        }
    }
}
