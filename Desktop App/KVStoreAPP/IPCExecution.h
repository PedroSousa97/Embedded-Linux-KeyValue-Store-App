#ifndef __IPCEXECUTION_H__
#define __IPCEXECUTION_H__

//CONSOLE OUTPUT COLORS
#define CNORMAL "\x1B[0m"   //Default color
#define LIGHTRED "\x1B[91m" //Red - for DB operations error messages
#define GREEN "\x1B[32m"    //Green - for DB operations success messages

void requestHandler(KVDataBase DataBase);

#endif //  __IPCEXECUTION_H__