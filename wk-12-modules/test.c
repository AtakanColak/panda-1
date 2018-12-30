#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int main(int n, char *args[n]) {
    setbuf(stdout, NULL);
    printf("Tests passed : %d\n", testLists());
    return 0;
}
