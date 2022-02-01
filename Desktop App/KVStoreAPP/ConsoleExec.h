#ifndef __CONSOLEEXEC_H__
#define __CONSOLEEXEC_H__

//CONSOLE OUTPUT COLORS
#define CNORMAL "\x1B[0m"   //Default color
#define LIGHTRED "\x1B[91m" //Red - for DB operations error messages
#define GREEN "\x1B[32m"    //Green - for DB operations success messages

void consoleExecution(KVDataBase DataBase);

#endif // __CONSOLEEXEC_H__