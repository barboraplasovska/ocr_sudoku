#include <stdio.h>
#include <stdlib.h>
#include "functions.h"

int main(int argc,char** argv)
{
    
    (void)argc;
    load(argv[1]);
    solve();
    finalProduct(argv[1]);

    return 0;
}
