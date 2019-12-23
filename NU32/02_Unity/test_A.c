#include <stdio.h>
#include <string.h>
#include "unity.h"

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning
    
    UNITY_BEGIN();
    printf("Test A\n\r");

    return UNITY_END();
}
