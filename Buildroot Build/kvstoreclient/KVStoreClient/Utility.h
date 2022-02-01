#ifndef UTILITY_H
#define UTILITY_H

enum eErrorType
{
    SUCCESS,
    SOCKETOPENFAIL,
    BINDFAIL,
    LISTENFAIL,
    ACCEPTFAIL,
    SENDFAIL,
    RECEIVEFAIL,
    GETHOSTFAIL,
    CONNECTFAIL,
    WRITEFAIL,
    READFAIL,
    RENAMEFAIL,
    DELETEFAIL,
    FAIL
};

#endif