#include <stdio.h>
#include "SilentC.h"

char *testSrc = "x = 4 + 2 * 3 + 5; y = 5 + 5;";

int main(){
    SilentCompile(testSrc);
    printf("Hello\n");
    return 0;
}