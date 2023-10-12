#include "headers.h"


struct message* set_message(int type, char*msg)
{
    struct message m;
    m.type=type;
    strcpy(m.msg,msg);

    return &m;
}