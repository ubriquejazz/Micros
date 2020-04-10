#include <stdio.h>
#include <string.h>
#include "unity.h"

COMMAND_DATA command;

void init_Command(int argc, const char* name)
{
   command.argv[0]= name; 
   command.argc = argc;
}

/***** Setters ****/

void test_Cmd_Set_Power_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "All the transmitters set to 250\r\n";

   init_Command(3, "SET_POWER"); 
   command.argv[1] = "4";
   command.argv[2] = "250";

   Cmd_Set_Power(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_Cmd_Set_Speed_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "All the banks set to 250\r\n";

   init_Command(4, "SET_SPEED"); 
   command.argv[1] = "4";
   command.argv[2] = "250";

   Cmd_Set_Speed(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_Cmd_Set_Power_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "SET_POWER <irda_ch> <pwm>\r\n";

   init_Command(1, "SET_POWER");   
   Cmd_Set_Power(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);

   init_Command(2, "SET_POWER");   
   Cmd_Set_Power(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_Cmd_Set_Speed_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "SET_SPEED <bank> <pwm>\r\n";

   init_Command(1, "SET_SPEED");   
   Cmd_Set_Speed(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);

   init_Command(2, "SET_SPEED");   
   Cmd_Set_Speed(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);     
}

/***** Getters *****/

void test_Cmd_Get_Power_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "[ERROR] ReadAnalog command\r\n";

   init_Command(2, "GET_POWER"); 
   command.argv[1] = "4";
   
   Cmd_Get_Power(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_Cmd_Get_Speed_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "[None] 0 0 0 0 0\r\n";

   init_Command(2, "GET_SPEED"); 
   command.argv[1] = "4";
   
   Cmd_Get_Speed(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_Cmd_Get_Power_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "GET_POWER <irda_ch>\r\n";

   init_Command(1, "GET_POWER");   
   Cmd_Get_Power(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);
}

void test_Cmd_Get_Speed_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "GET_SPEED <bank>\r\n";

   init_Command(1, "GET_SPEED");   
   Cmd_Get_Speed(&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);
}

/**** IRDA Local Commands - Channels 1:5 ****/

void test_Cmd_Channel_BeaconMR_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[MAX_NUM_OF_BYTES];
   
   init_Command(2, "LCL_MR"); 
   command.argv[1] = "5";
   Cmd_Ch_BeaconMR(&command, actual);  

   sprintf(expected, "Sent: $MR5 1234\r\n");     // not defined PROD0
   TEST_ASSERT_EQUAL_STRING(expected, actual);    
}

void test_Cmd_Channel_BeaconMS_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];   
   uint8_t expected[MAX_NUM_OF_BYTES];
   
   init_Command(2, "LCL_MS"); 
   command.argv[1] = "1";
   Cmd_Ch_BeaconMS(&command, actual);
     
   sprintf(expected, "Sent: $MS1 1234 %s\r\n", CONSOLE_EXAMPLE_COMMAND);
   TEST_ASSERT_EQUAL_STRING(expected, actual); 
}
    
void test_Cmd_Channel_Beacons_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[MAX_NUM_OF_BYTES];
   uint8_t tail[] = "<irda_ch>\r\n";

   init_Command(1, "LCL_MR");   
   Cmd_Ch_BeaconMR (&command, actual);
   sprintf (expected, "LCL_MR %s", tail);
   TEST_ASSERT_EQUAL_STRING(expected, actual);

   init_Command(1, "LCL_MS");   
   Cmd_Ch_BeaconMS (&command, actual);
   sprintf (expected, "LCL_MS %s", tail);   
   TEST_ASSERT_EQUAL_STRING(expected, actual);    
}

/**** FAN Remote Commands - Addresses 0:9 ****/

void test_Cmd_Address_BeaconMR_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[MAX_NUM_OF_BYTES];

   init_Command(2, "RMT_MR"); 
   command.argv[1] = "3";
   Cmd_Addr_BeaconMR(&command, actual);  

   sprintf(expected, "Address 3 is busy\r\n");
   TEST_ASSERT_EQUAL_STRING(expected, actual);    
}

void test_Cmd_Address_BeaconMS_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];   
   uint8_t expected[MAX_NUM_OF_BYTES];

   init_Command(2, "RMT_MS"); 
   command.argv[1] = "8";
   Cmd_Addr_BeaconMS(&command, actual);
     
   sprintf(expected, "Address 8 is busy\r\n"); 
   TEST_ASSERT_EQUAL_STRING(expected, actual); 
}

void test_Cmd_Address_Beacons_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[MAX_NUM_OF_BYTES];
   uint8_t tail[] = "<irda_addr>\r\n";

   init_Command(1, "RMT_MR");   
   Cmd_Addr_BeaconMR (&command, actual);
   sprintf (expected, "RMT_MR %s", tail);
   TEST_ASSERT_EQUAL_STRING(expected, actual);

   init_Command(1, "RMT_MS");   
   Cmd_Addr_BeaconMR (&command, actual);
   sprintf (expected, "RMT_MS %s", tail);   
   TEST_ASSERT_EQUAL_STRING(expected, actual);    
}

/**/

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning

   printf("\n\nTest Component Simulation PWM\n\r");

    UNITY_BEGIN();
    
    printf("Setter functions\n\r");
    RUN_TEST(test_Cmd_Set_Power_ResultOk);
    RUN_TEST(test_Cmd_Set_Speed_ResultOk);
    RUN_TEST(test_Cmd_Set_Power_MissingArgs);
    RUN_TEST(test_Cmd_Set_Speed_MissingArgs);    
    
    printf("Getter functions\n\r");
    RUN_TEST(test_Cmd_Get_Power_ResultOk);
    RUN_TEST(test_Cmd_Get_Speed_ResultOk);
    RUN_TEST(test_Cmd_Get_Power_MissingArgs);
    RUN_TEST(test_Cmd_Get_Speed_MissingArgs);
    
    printf("Command to trigger IR messages (localy) \r\n");
    RUN_TEST(test_Cmd_Channel_BeaconMR_ResultOk);
    RUN_TEST(test_Cmd_Channel_BeaconMS_ResultOk);
    RUN_TEST(test_Cmd_Channel_Beacons_MissingArgs);

    printf("Command to trigger IR messages (remotely) \r\n");
    RUN_TEST(test_Cmd_Address_BeaconMR_ResultOk);
    RUN_TEST(test_Cmd_Address_BeaconMS_ResultOk);
    RUN_TEST(test_Cmd_Address_Beacons_MissingArgs);

    return UNITY_END();
}

