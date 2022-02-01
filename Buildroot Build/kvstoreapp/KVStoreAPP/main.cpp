#include <iostream>
#include <string>
#include <assert.h>
#include <cassert>
#include "leveldb/db.h"
#include "KVDataBase.h"
#include "ConsoleExec.h"

using namespace std;

int main(void)
{
    //instantiate the KVDataBase
    KVDataBase DB;

    //Start console execution
    consoleExecution(DB);

    //If user wants to close application, also close Database
    DB.closeDB();

    return 0;
}
