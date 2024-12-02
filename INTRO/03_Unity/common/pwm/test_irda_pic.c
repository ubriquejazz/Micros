#include <stdio.h>
#include <string.h>
#include "unity.h"

COMMAND_DATA command;

void init_Command(int argc, const char* name)
{
   command.argv[0]= name; 
   command.argc = argc;
}

extern uint8_t Scheduled_Message[5][MAX_NUM_OF_BYTES];
extern uint8_t Scheduled_Channel[];
extern uint8_t infrared_silence[];

/* IRDA_Add_Beacon - INTEGRATION TEST! */

void test_IRDA_Add_Beacon_BT(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[MAX_NUM_OF_BYTES];

   BLADE_ResetAll();
   uint8_t i = 3;   // device
   uint8_t addr = channel2address(i);
   uint16_t id = 0xABCD;
   // for (i=1; i<=5; i++)
   {
      BLADE_WriteID(i, id);
      BLADE_Uncalibrate(i);

      sprintf(expected, CONSOLE_IR_REQ_BT, addr);
      AppendCrc(expected);

      IRDA_Add_Beacon(i, 0, 'A', actual);
      TEST_ASSERT_EQUAL_STRING(expected, actual);
   }
}

void test_IRDA_Add_Beacon_BC(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[MAX_NUM_OF_BYTES];

   BLADE_ResetAll();
   uint8_t i = 3; // device
   uint8_t addr = channel2address(i);
   uint16_t id = 0xABCD;
   // for (i=1; i<=5; i++)
   {
      BLADE_WriteID(i, id);
      BLADE_Calibrate(i);

      sprintf(expected, CONSOLE_IR_REQ_BC, addr, id);
      AppendCrc(expected);

      IRDA_Add_Beacon(i, 0, 'A', actual);
      TEST_ASSERT_EQUAL_STRING(expected, actual);
    }
}

void test_IRDA_Add_BeaconAndMessage_MS(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[MAX_NUM_OF_BYTES];
   uint8_t beacon[MAX_NUM_OF_BYTES];
   uint8_t message[MAX_NUM_OF_BYTES];

   BLADE_ResetAll();
   uint8_t i = 3; // device
   uint8_t addr = channel2address(i);
   uint16_t id = 0xABCD;
   // for (i=1; i<=5; i++)
   {      
      BLADE_WriteID(i, id);
      BLADE_Calibrate(i);
      
      sprintf(beacon, CONSOLE_IR_REQ_BC, addr, id);
      AppendCrc(beacon);
      sprintf(expected, "%s\r%s", beacon, infrared_silence);  
      
      IRDA_Add_BeaconAndMessage('S', i, actual);
      TEST_ASSERT_EQUAL_STRING(expected, actual);
      
      sprintf(message, CONSOLE_IR_REQ_MS, addr, BLADE_ReadID(i), CONSOLE_EXAMPLE_COMMAND);
      AppendCrc(message);
      TEST_ASSERT_EQUAL_STRING(message, Scheduled_Message[i-1]);
   }
}

void test_IRDA_Add_BeaconAndMessage_MR(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[MAX_NUM_OF_BYTES];
   uint8_t beacon[MAX_NUM_OF_BYTES];
   uint8_t message[MAX_NUM_OF_BYTES];
   
   BLADE_ResetAll();
   uint8_t i = 3; // device
   uint8_t addr = channel2address(i);
   uint16_t id = 0xABCD;
   // for (i=1; i<=5; i++)
   {
      BLADE_WriteID(i, id);
      BLADE_Calibrate(i);

      sprintf(beacon, CONSOLE_IR_REQ_BC, addr, id);
      AppendCrc(beacon);
      sprintf(expected, "%s\r%s", beacon, infrared_silence);  

      IRDA_Add_BeaconAndMessage('R', i, actual);
      TEST_ASSERT_EQUAL_STRING(expected, actual);

      sprintf(message, CONSOLE_IR_REQ_MR, addr, BLADE_ReadID(i)); 
      AppendCrc(message);
      TEST_ASSERT_EQUAL_STRING(message, Scheduled_Message[i-1]);
   }
}

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning
    
    UNITY_BEGIN();

    printf("\n\r");
    printf("IRDA_Add_Beacon (INTEGRATION TEST!)\r\n");
    RUN_TEST(test_IRDA_Add_Beacon_BT);
    RUN_TEST(test_IRDA_Add_Beacon_BC);
    RUN_TEST(test_IRDA_Add_BeaconAndMessage_MS);
    RUN_TEST(test_IRDA_Add_BeaconAndMessage_MR);
    
    return UNITY_END();
}
