#include <stdio.h>
#include <string.h>
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


void test_HB_BeaconsAndCallbacks_ResultOk(void)
{
    uint8_t read_value, i, j;
    HB_Reset(); // Ones
    
    for (j=0; j<10; j++)
    {
        for (i=1; i<=5; i++)
        {       
            HB_NewBeacon(i); 
            read_value = HB_ReadValue(i);
            TEST_ASSERT_EQUAL(read_value, 1);
        }

        for (i=1; i<=5; i++)
        {       
            HB_NewCallback(i);
            read_value = HB_ReadValue(i);
            TEST_ASSERT_EQUAL(read_value, 0);
        }
    }
}

void test_HB_ConsecutiveBeacons_Calibrated(void)
{
    uint8_t read_value, i, j;
   
    for (i=1; i<=5; i++)
    {
        BLADE_Calibrate(i);
        read_value = HB_ConsecutiveBeacons(i, HB_NUM_RETRIES+1);
        TEST_ASSERT_EQUAL(read_value, HB_NUM_RETRIES);   
        read_value = HB_ReadValue(i);
        TEST_ASSERT_EQUAL(read_value, 1);     
    }

    for (i=1; i<=5; i++)
    {    
        read_value = BLADE_ReadCalibrated(i);
        TEST_ASSERT_EQUAL(read_value, 1);  
    } 
}

void test_HB_ConsecutiveBeacons_Uncalibrated(void)
{
    uint8_t read_value, i, j;
    
    for (i=1; i<=5; i++)
    {
        BLADE_Calibrate(i);
        read_value = HB_ConsecutiveBeacons(i, HB_NUM_RETRIES+2);
        TEST_ASSERT_EQUAL(read_value, HB_NUM_RETRIES);   
        read_value = HB_ReadValue(i);
        TEST_ASSERT_EQUAL(read_value, 1);     
    }

    for (i=1; i<=5; i++)
    {    
        read_value = BLADE_ReadCalibrated(i);
        TEST_ASSERT_EQUAL(read_value, 0);  
    } 
}

int main(int argc, char** argv)
{
    (void) argc; // Cast to void to avoid warning
    (void) argv; // Cast to void to avoid warning

    status.irdaStatus = 0x01;

    UNITY_BEGIN();

    printf("Blade ID\n\r");
    RUN_TEST(test_BLADE_WriteID_OneIteration);
    RUN_TEST(test_BLADE_WriteID_TwoIteration);
    RUN_TEST(test_BLADE_WriteReadState);

    printf("Blade detection\n\r");    
    RUN_TEST(test_HB_BeaconsAndCallbacks_ResultOk);
    RUN_TEST(test_HB_ConsecutiveBeacons_Calibrated);
    RUN_TEST(test_HB_ConsecutiveBeacons_Uncalibrated);

    return UNITY_END();
}
