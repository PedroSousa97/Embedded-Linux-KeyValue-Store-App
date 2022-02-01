#include <iostream>
#include <bits/stdc++.h>
#include <cassert>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "leveldb/db.h"
#include "KVDataBase.h"
#include "ServerOBJ.h"
#include "IPCExecution.h"

using namespace std;

void checkRequest(vector<std::string> input, KVDataBase DataBase, Server *serObj);
void ReceiveCommand(Server *serObj, std::string &receivedcmd);
void PrintErrorMSg(std::string errMsg);

//Main socket creation and request logic handler function
void requestHandler(KVDataBase DataBase)
{
    //instantiate the Server
    Server serObj;

    //Begin with success code and change later on depending on the result of the requests
    //and socket operations
    eErrorType Error = SUCCESS;

    //Prepare string that will hold the client's messages
    std::string receivedcmd;

    //Open Socket
    Error = serObj.OpenSocket();
    if (Error != SUCCESS)
    {
        cout << "socket open fail" << endl;
        cout << "Going back to Input mode :)" << endl
             << endl;
        //Go back to input mode
        return;
    }

    cout << GREEN << "Socket created successfully." << CNORMAL << endl;

    //Bind socket
    Error = serObj.BindServer();
    if (Error != SUCCESS)
    {
        PrintErrorMSg("Server Bind Fail");
        return;
    }

    cout << GREEN << "Bind socket successfully." << CNORMAL << endl;

    //Socket listening for clients
    Error = serObj.ListenServer();
    if (Error != SUCCESS)
    {
        PrintErrorMSg("Server Listen Fail");
        return;
    }

    cout << GREEN << "Socket Listening" << CNORMAL << endl;

    //Accept a client
    Error = serObj.AcceptServer();
    if (Error == SUCCESS)
    {
        cout << "Server got connection from " << inet_ntoa(serObj.cli_addr.sin_addr) << " port " << ntohs(serObj.cli_addr.sin_port) << endl;
    }

    cout << GREEN << "Client Connected! Waiting for commands :)" << CNORMAL << endl;

    //While the client wants to continue using KVStoreAPP, keep waiting for commands
    //only exit if the client sends (EXIT_IPC)
    while (receivedcmd != "(EXIT_IPC)")
    {
        //Receive command from client
        ReceiveCommand(&serObj, receivedcmd);
        //Print command for debugging purposes
        cout << "Received command :" << receivedcmd << endl;

        //When the client unexpectadly closes/terminates connection, an empty command is received
        //Here I protect the execution against it, otherwise we would have segmentation fault
        if (receivedcmd.empty())
        {
            cout << "Unexpected disconnection from client :(" << endl;
            cout << "Going back to Input mode :)" << endl
                 << endl;
            //Client terminated the connection unexpectadly, then close socket, and return to Input mode
            serObj.CloseSocket();
            receivedcmd = "(EXIT_IPC)";
            return;
        }

        // Make a string-based stream from the line entered by the user
        istringstream iss(receivedcmd);
        // Prepare a vector of strings to split the input
        vector<std::string> splitInput;
        // I could use a loop, but using an iterator is more idiomatic to C++
        istream_iterator<string> iit(iss);
        // back_inserter will add the items one by one to the vector splitInput
        copy(iit, istream_iterator<string>(), back_inserter(splitInput));

        //Check if the client wants to stop execution
        if (splitInput[0] == "(EXIT_IPC)")
        {
            cout << "Going back to Input mode :)" << endl
                 << endl;
            //Clientwants to terminate connection, then close socket, and return to Input mode
            serObj.CloseSocket();
            receivedcmd = "(EXIT_IPC)";
            return;
        } //if the user doesn't want to close the connection, then handle command
        else
        {
            //Function that handles the different commands sent from client
            checkRequest(splitInput, DataBase, &serObj);
        }
    }
}

//Simple function used to print errors
void PrintErrorMSg(std::string errMsg)
{
    cout << errMsg << endl;
}

//Function used to receive the client message and copy it to the receivedcmd variable
void ReceiveCommand(Server *serObj, std::string &receivedcmd)
{
    //start with a success "Error"
    eErrorType Error = SUCCESS;

    Error = serObj->ReceiveServer();
    if (Error != SUCCESS)
    {
        PrintErrorMSg("Server  Fail");
    }
    //Print client command/message
    cout << "Received msg :" << serObj->buffer << endl;
    //Copy client message content to the receivedcmd variable
    receivedcmd = serObj->buffer;
}

//Function used to handle the entire request commands from the client
void checkRequest(vector<std::string> input, KVDataBase DataBase, Server *serObj)
{
    eErrorType Error = SUCCESS;
    //Check if the client sent a SET command
    if (input[0] == "SET")
    {
        // If he did sent a SET command, it should have at least 3 arguments
        if (input.size() >= 3)
        {
            //Insert key-Pair value to the Database using the object SET method.
            DataBase.SET(input[1], input[2]);

            //Check if the Database operation went as expected, if it didn't inform the client
            if (!DataBase.getStatus().ok())
            {
                cout << "Sorry, " << LIGHTRED << "something went wrong" << CNORMAL << " when trying to insert the key - value pair." << endl;
                cout
                    << "Please try once again :)" << endl;
                //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
                Error = serObj->SendServer("Sorry, something went wrong when trying to insert the key - value pair. Please try again :)");
                if (Error != SUCCESS)
                {
                    PrintErrorMSg("Server Send Fail");
                }
            } // Insertion successful
            else
            {
                cout << "Key: " << input[1] << " and value: " << input[2] << GREEN << " successfully stored." << CNORMAL << endl;
                //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
                Error = serObj->SendServer("Key: " + input[1] + " and value: " + input[2] + " successfully stored.");
                if (Error != SUCCESS)
                {
                    PrintErrorMSg("Server Send Fail");
                }
            }
        }
        else
        { //This is the case where the client didn't input sufficient arguments for the SET method
            cout << "Sorry, I really need both parameters of the SET command to be able to store something." << endl;
            cout << "Please try once again :)" << endl;
            //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
            Error = serObj->SendServer("Sorry, I really need both parameters of the SET command to be able to store something.");
            if (Error != SUCCESS)
            {
                PrintErrorMSg("Server Send Fail");
            }
        }
    } //Check if the client sent a GET command
    else if (input[0] == "GET")
    {
        // If he did sent a GET command, it should have at least 2 arguments
        if (input.size() >= 2)
        {
            //GET the value of specified key from the Database and store it to later print to the client
            std::string value = DataBase.GET(input[1]);

            //Check if the Database operation went as expected, if it didn't inform the console client
            if (!DataBase.getStatus().ok())
            {
                cout << "Sorry, " << LIGHTRED << "I couldn't get the value for this key." << CNORMAL << " Are you sure it exists?" << endl;
                cout << "Please try once again :)" << endl;
                //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
                Error = serObj->SendServer("Sorry, I couldn't get the value for this key. Are you sure it exists?. Please try again :)");
                if (Error != SUCCESS)
                {
                    PrintErrorMSg("Server Send Fail");
                }
            }
            else
            {
                //GET request successful, print the value to the console client
                cout << GREEN << value << CNORMAL << endl;
                //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
                Error = serObj->SendServer(value);
                if (Error != SUCCESS)
                {
                    PrintErrorMSg("Server Send Fail");
                }
            }
        }
        else
        {
            //This is the case where the client didn't input sufficient arguments for the GET method
            cout << "Sorry, I really need the Key parameters of the GET command to be able to retrieve a value." << endl;
            cout << "Please try once again :)" << endl;
            //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
            Error = serObj->SendServer("Sorry, I really need the Key parameters of the GET command to be able to retrieve a value.");
            if (Error != SUCCESS)
            {
                PrintErrorMSg("Server Send Fail");
            }
        }
    } //Check if the client sent a DELETE command
    else if (input[0] == "DELETE")
    {
        // If he did sent a DELETE command, it should have at least 2 arguments
        if (input.size() >= 2)
        {
            //First check if the key exists, leveldb doesn't throw a status error for non existent key Deletes
            DataBase.GET(input[1]);

            //If the key doesn't exist inform the client and cancel the Delete operation, it is no longer necessary
            if (!DataBase.getStatus().ok())
            {
                cout << "Sorry, " << LIGHTRED << "something went wrong" << CNORMAL << " when trying to Delete this key-value pair. Are you sure it exists?" << endl;
                cout << "Please try once again :)" << endl;
                //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
                Error = serObj->SendServer("Sorry,something went wrong when trying to Delete this key-value pair. Are you sure it exists?. Please try again :)");
                if (Error != SUCCESS)
                {
                    PrintErrorMSg("Server Send Fail");
                }
            }
            else
            {
                //Otherwise if the key exists, then proceed with the Delete operation using Database object Delete method
                DataBase.DELETE(input[1]);

                //Check if the Database operation went as expected, if it didn't inform the client
                if (!DataBase.getStatus().ok())
                {
                    cout << "Sorry, something went wrong when trying to Delete this key-value pair. Are you sure it exists?" << endl;
                    cout << "Please try once again :)" << endl;
                    //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
                    Error = serObj->SendServer("Sorry,something went wrong when trying to Delete this key-value pair. Are you sure it exists?. Please try again :)");
                    if (Error != SUCCESS)
                    {
                        PrintErrorMSg("Server Send Fail");
                    }
                }
                else
                {
                    // Delete successful
                    cout << "Key-Value pair deleted " << GREEN << "successfully." << CNORMAL << endl;
                    //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
                    Error = serObj->SendServer("Key-Value pair deleted successfully.");
                    if (Error != SUCCESS)
                    {
                        PrintErrorMSg("Server Send Fail");
                    }
                }
            }
        }
        else
        {
            //This is the case where the client didn't input sufficient arguments for the DELETE method
            cout << "Sorry, I really need the Key parameters of the DELETE command to be able to delete" << endl;
            cout << "the key-value pair successfully." << endl;
            cout << "Please try once again :)" << endl;
            //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
            Error = serObj->SendServer("Sorry, I really need the Key parameters of the DELETE command.");
            if (Error != SUCCESS)
            {
                PrintErrorMSg("Server Send Fail");
            }
        }
    }
    else
    {
        //This is the case where the client input an invalid command, like PUT instead of SET. Inform the client the command doesn't exist
        cout << "Please, use one of the valid request commands. Other commands won't be accepted" << endl;
        //Not only print result but also send it back to the client. This is a request->acknowlegde type connection, client waits for response
        Error = serObj->SendServer("Please, use one of the valid request commands. Other commands won't be accepted.");
        if (Error != SUCCESS)
        {
            PrintErrorMSg("Server Send Fail");
        }
    }
}