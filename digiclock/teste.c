#include <stdio.h>
#include "includes/myargs.h"

int main()
{
    char *argv ="-t";

    for (int i = 0; i < 2; i++)
        printf("%s\n", trimArg(argv));

    return 0;
}