#include <bits/stdc++.h>
#include <cassert>
#include <iostream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include "leveldb/db.h"
#include "KVDataBase.h"
#include "ConsoleExec.h"
#include "IPCExecution.h"

using namespace std;

void checkConsoleInput(vector<std::string> input, KVDataBase DataBase);

//Main console execution function
void consoleExecution(KVDataBase DataBase)
{
    cout << "********************************* KVStoreApp **********************************" << endl;
    cout << "This is a persistent in disk Key-Value pair storage console application." << endl;
    cout << "Having that into account you got the following request available:" << endl;
    cout << "SET [key] [value] -> Store a new Key-Value pair to the disk; " << endl;
    cout << "GET [key] [value] -> GET the value of a secific key, if it exists; " << endl;
    cout << "DELETE [key] [value] -> DELETE Key-Value pair from storage, with specified key;" << endl
         << endl;
    cout << "Requests Commands other than the ones specified will be ingored, as well as " << endl;
    cout << "command parameters additional to the ones expected." << endl
         << endl;
    cout << "If you want to change from console input to IPC execution mode just type (IPC)." << endl;
    cout << "If using IPC, to inform the application that you want to go back to Input mode" << endl;
    cout << "the other process must send an (EXIT_IPC) command." << endl;
    cout << "If you want to close this application just type (EXIT), including parenthesis." << endl;
    cout << "The (EXIT) command can only be sent by the console user." << endl;
    cout << "Have fun! That's what coding and computer interaction is all about! :)" << endl;
    cout << "*******************************************************************************" << endl;

    // Prepare the buffer for the line the user enters
    std::string cinRequest;
    // Int variable used to changed between input and IPC execution modes
    int isIPC = 1;

    //wait for the user to input (Exit) to close/end execution
    while (cinRequest != "(EXIT)")
    {
        // This buffer will grow automatically to accommodate the entire input line
        getline(cin, cinRequest);

        //Check if the user actually input anything, otherwise it would result in a segmentation faultsss
        if (cinRequest.empty())
        {
            cout << "Sorry, but I think " << LIGHTRED << "you didn't input anything." << endl;
            cout << CNORMAL << "Please try once again :)" << endl;
        }
        else
        {
            // Make a string-based stream from the line entered by the user
            istringstream iss(cinRequest);
            // Prepare a vector of strings to split the input
            vector<std::string> splitInput;
            // I could use a loop, but using an iterator is more idiomatic to C++
            istream_iterator<string> iit(iss);
            // back_inserter will add the items one by one to the vector splitInput
            copy(iit, istream_iterator<string>(), back_inserter(splitInput));

            //Check if the user wants to stop execution
            if (splitInput[0] == "(EXIT)" && isIPC == 1)
            {

                cout << "Hope you enjoyed it :) Closing Database and Application. See you next time!" << endl;
            } //Check if the user wants to change to IPC execution mode
            else if (splitInput[0] == "(IPC)" && isIPC == 1)
            {
                isIPC = 2;
                cout << "Changing to IPC mode, please connect to the application using IPC." << endl;
                cout << "With another process, you can send the exact same commands and I" << endl;
                cout << "will process them and send you the respective response." << endl
                     << endl;
                requestHandler(DataBase);
                isIPC = 1;
            } //If the user didn't ask to close the application neither changed execution mode, inspect request input
            else if (isIPC == 1)
            {
                //Function that handles the different inputs from the console user
                checkConsoleInput(splitInput, DataBase);

            } //This is the IPC execution mode case
        }
    }
}

//Function used to handle the entire request input logic from the console user
void checkConsoleInput(vector<std::string> input, KVDataBase DataBase)
{
    //Check if the user sent a SET command
    if (input[0] == "SET")
    {
        // If he did sent a SET command, it should have at least 3 arguments
        if (input.size() >= 3)
        {
            //Insert key-Pair value to the Database using the object SET method.
            DataBase.SET(input[1], input[2]);

            //Check if the Database operation went as expected, if it didn't inform the console user
            if (!DataBase.getStatus().ok())
            {
                cout << "Sorry, " << LIGHTRED << "something went wrong" << CNORMAL << " when trying to insert the key - value pair." << endl;
                cout
                    << "Please try once again :)" << endl;
            } // Insertion successful
            else
            {
                cout << "Key: " << input[1] << " and value: " << input[2] << GREEN << " successfully stored." << CNORMAL << endl;
            }
        }
        else
        { //This is the case where the user didn't input sufficient arguments for the SET method
            cout << "Sorry, I really need both parameters of the SET command to be able to store something." << endl;
            cout << "Please try once again :)" << endl;
        }
    } //Check if the user sent a GET command
    else if (input[0] == "GET")
    {
        // If he did sent a GET command, it should have at least 2 arguments
        if (input.size() >= 2)
        {
            //GET the value of specified key from the Database and store it to later print to the user
            std::string value = DataBase.GET(input[1]);

            //Check if the Database operation went as expected, if it didn't inform the console user
            if (!DataBase.getStatus().ok())
            {
                cout << "Sorry, " << LIGHTRED << "I couldn't get the value for this key." << CNORMAL << " Are you sure it exists?" << endl;
                cout << "Please try once again :)" << endl;
            }
            else
            {
                //GET request successful, print the value to the console user
                cout << GREEN << value << CNORMAL << endl;
            }
        }
        else
        {
            //This is the case where the user didn't input sufficient arguments for the GET method
            cout << "Sorry, I really need the Key parameters of the GET command to be able to retrieve a value." << endl;
            cout << "Please try once again :)" << endl;
        }
    } //Check if the user sent a DELETE command
    else if (input[0] == "DELETE")
    {
        // If he did sent a DELETE command, it should have at least 2 arguments
        if (input.size() >= 2)
        {
            //First check if the key exists, leveldb doesn't throw a status error for non existent key Deletes
            DataBase.GET(input[1]);

            //If the key doesn't exist inform the user and cancel the Delete operation, it is no longer necessary
            if (!DataBase.getStatus().ok())
            {
                cout << "Sorry, " << LIGHTRED << "something went wrong" << CNORMAL << " when trying to Delete this key-value pair. Are you sure it exists?" << endl;
                cout << "Please try once again :)" << endl;
            }
            else
            {
                //Otherwise if the key exists, then proceed with the Delete operation using Database object Delete method
                DataBase.DELETE(input[1]);

                //Check if the Database operation went as expected, if it didn't inform the console user
                if (!DataBase.getStatus().ok())
                {
                    cout << "Sorry, something went wrong when trying to Delete this key-value pair. Are you sure it exists?" << endl;
                    cout << "Please try once again :)" << endl;
                }
                else
                {
                    // Delete successful
                    cout << "Key-Value pair deleted " << GREEN << "successfully." << CNORMAL << endl;
                }
            }
        }
        else
        {
            //This is the case where the user didn't input sufficient arguments for the DELETE method
            cout << "Sorry, I really need the Key parameters of the DELETE command to be able to delete" << endl;
            cout << "the key-value pair successfully." << endl;
            cout << "Please try once again :)" << endl;
        }
    }
    else
    {
        //This is the case where the user input an invalid command, like PUT instead of SET. Inform the user the command doesn't exist
        cout << "Please, use one of the valid request commands. Other commands won't be accepted" << endl;
    }
}