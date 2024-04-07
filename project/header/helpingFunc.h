#ifndef _HELPING_FUNC_
#define _HELPING_FUNC_

#include <ctype.h>

int digits_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }

    return 1;
}

#endif //_HELPING_FUNC_