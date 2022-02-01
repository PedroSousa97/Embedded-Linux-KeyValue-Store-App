#ifndef __KVDATABASE_H__
#define __KVDATABASE_H__

/*
*   Database class declaration in header file
*   Defining the Database as an object will give me better
*   control over the DB, data and overall needed methods
*/
class KVDataBase
{
private:
    //pointer that will store the db object
    leveldb::DB *db;
    //Options object to store the options for creating/opening DB
    leveldb::Options options;
    //ReadOptions object that is used to GET data from the DB
    leveldb::ReadOptions readOptions;
    //WriteOptions object that is used to SET and DELETE data from the DB
    leveldb::WriteOptions writeOptions;
    //Status object that is used to debug any malfunction or missing keys
    leveldb::Status status;
    //Private method that is only used by the class constructor to build the DB file location
    std::string createDir();

public:
    //Class cunstroctor
    KVDataBase();
    //Method used to access and return the current DB status (Usual Getter)
    leveldb::Status getStatus();
    //Method used to insert data to the DB
    void SET(std::string key, std::string value);
    //Method used to get data from the DB, return value from specific key
    std::string GET(std::string key);
    //Method used to delete a specific key-value pair from the Database
    void DELETE(std::string key);
    //method used to close the DB before ending the application execution
    void closeDB();
};

#endif // __KVDATABASE_H__