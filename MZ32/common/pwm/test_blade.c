#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "unity.h"

IRDA_STATUS_BYTE status;
uint16_t faked_id = 0xABCD;

void test_BLADE_WriteID_OneIteration()
{
    BLADE_ResetAll(); 
    uint8_t i;
    for (i=1; i<=5; i++)
    {       
        BLADE_WriteID(i, faked_id);
        TEST_ASSERT_EQUAL (BLADE_ReadCounter(i), 1);
    }
}

void test_BLADE_WriteID_TwoIteration()
{
    BLADE_ResetAll(); 
    uint8_t i;
    for (i=1; i<=5; i++)
    {  
        BLADE_WriteID(i, faked_id);
        BLADE_WriteID(i, faked_id);
        TEST_ASSERT_EQUAL (BLADE_ReadCounter(i), 2);
    }
}

void test_BLADE_WriteReadState()
{
    BLADE_ResetAll(); 
    uint8_t i, local;
    for (i=1; i<=5; i++)
    {
        BLADE_WriteState(i, status);
        local = BLADE_ReadState(i);
        TEST_ASSERT_EQUAL(local, 0x01);
    }
}

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning
        
    status.irdaStatus = 0x01;
    
    UNITY_BEGIN();
    
    printf("\n\r");
    RUN_TEST(test_BLADE_WriteID_OneIteration);
    RUN_TEST(test_BLADE_WriteID_TwoIteration);
    RUN_TEST(test_BLADE_WriteReadState);

    return UNITY_END();
}
