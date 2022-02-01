#include <bits/stdc++.h>
#include <cassert>
#include <iostream>
#include <string>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "leveldb/db.h"
#include "KVDataBase.h"

using namespace std;

//Class constructor
KVDataBase::KVDataBase()
{
    //Set the options create_if_missing to true, otherwise it won't create new DBs and will crash
    this->options.create_if_missing = true;
    //While trying to create/open DB, save the result (success or failure) in the status
    this->status = leveldb::DB::Open(this->options, createDir(), &db);
    //If something goes wrong, the progra execution is closed by the assertion
    assert(this->status.ok());
}

//Private method that is only used by the class constructor to build the DB file location
std::string KVDataBase::createDir()
{
    char *homeDir;

    //Get the current user home path, so that no permission elevation is needed to create the DB
    if ((homeDir = getenv("HOME")) == NULL)
    {
        homeDir = getpwuid(getuid())->pw_dir;
    }

    //New folder name used to create the new directory that will store the DB
    char const *folderName = "/PSData";
    //helper that will hold the user specific home path concatenated with the new folder name
    char *newDir = new char[strlen(homeDir) + strlen(folderName) + 1];
    //Finally concatenating the new folder path, to be used to mkdir in the following code
    strcpy(newDir, homeDir);
    strcat(newDir, folderName);

    /*levelDB won't accept the DB final path in a data tyoe other than string.
    Having that into account, here we simply convert the final path to string
    and store it under PATH std::string variable*/
    std::string PATH(newDir);

    //Generic method to check if the directory already exists, otherwise you won't override it
    struct stat st;

    if (stat(newDir, &st) != 0)
    {
        //Since it doesn't aready exist, create the new DB directory
        const int check = mkdir(newDir, 0777);

        // check if directory was created successfully or not
        if (!check)
        {
            cout << "Creating Database Directory under " << PATH << endl;
        }
        else
        {
            cerr << "Unable to Create Database Directory, make sure you have sufficient permissions..." << endl;
            exit(1);
        }
    }

    //Return full PATH in std::string format, will be useful to initialyze the Database
    return PATH;
}

//Simple getter method to access the current Database Satus. Useful to check successful or failed operations
leveldb::Status KVDataBase::getStatus()
{
    return this->status;
}

//Simple method to Insert the new key-value pair into the Database, using generic library functions
void KVDataBase::SET(std::string key, std::string value)
{
    //While inserting the key-value, save the insertion result (success or failure) in the status
    this->status = this->db->Put(this->writeOptions, key, value);
}

//Simple method to Get the value of a specific key from the Database
std::string KVDataBase::GET(std::string key)
{
    //variable that will hold the db->get request result
    std::string value;
    //While getting the value, save the get result (success or failure) in the status
    this->status = this->db->Get(this->readOptions, key, &value);
    //return the db->get request result
    return value;
}

//Simple method to Delete a key-value pair from the Database, using a specific key
void KVDataBase::DELETE(std::string key)
{
    //While deleting the key-value, save the delete result (success or failure) in the status
    this->status = this->db->Delete(this->writeOptions, key);
}

void KVDataBase::closeDB()
{
    //close the DB, you should always close the DB after using it
    delete this->db;
}