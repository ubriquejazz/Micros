#include <stdio.h>
#include <string.h>
#include "unity.h"

uint8_t response[MAX_NUM_OF_BYTES];

extern uint16_t BladeID[] ;
extern uint8_t  BladeState[];
extern uint8_t  Scheduled_Channel[];

COMMAND_DATA command;

void init_Command (int argc, const char* name)
{
   command.argv[0]= name; 
   command.argc = argc;
}

uint16_t getId (uint8_t i)
{
    return BLADE_ReadID(i);
}

/* IRDA_UpdateTable (AT) */

void TestInit_irdaStable (uint16_t id)
{
    IRDA_STATUS_BYTE status;
    status.irdaStatus = 0x01;

    BLADE_ResetAll();
    uint8_t i; // device
    for (i=1; i<=5; i++)
    {
        BLADE_WriteID(i, id);
        BLADE_WriteState(i, status);
    } 
}

void test_IRDA_UpdateTable_TwoIterations()
{
    TestInit_irdaStable(0xABCD); 
    uint8_t i;
    for (i=1; i<=5; i++)
    {
      uint8_t addr = channel2address(i);
      IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
      TEST_ASSERT_EQUAL (BLADE_ReadCounter(i), 2);
    }
}

void test_IRDA_UpdateTable_ThreeIterations()
{
    TestInit_irdaStable(0xABCD); 
    uint8_t i;
    for (i=1; i<=5; i++)
    {  
        uint8_t addr = channel2address(i);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        TEST_ASSERT_EQUAL (BLADE_ReadCounter(i), 3);
        TEST_ASSERT_EQUAL (BLADE_ReadCalibrated(i), 0);
    }
}

void test_IRDA_UpdateTable_FourIterationsAndCalibrated()
{
    TestInit_irdaStable(0xABCD); 
    uint8_t i;
    for (i=1; i<=5; i++)
    {  
        uint8_t addr = channel2address(i);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        TEST_ASSERT_EQUAL (BLADE_ReadCounter(i), 4);
        TEST_ASSERT_EQUAL (BLADE_ReadCalibrated(i), 1);
    }
}

void test_IRDA_UpdateTable_FourIterationsAndUncalibrate()
{
    TestInit_irdaStable(0xABCD); 
    uint8_t i = 3;
    uint16_t new_id = 0x1234;
    for (i=1; i<=5; i++)
    {
        uint8_t addr = channel2address(i);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);        
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);        
        IRDA_UpdateTable(addr, i, new_id,   BLADE_ReadState(i), response);
        TEST_ASSERT_EQUAL (BLADE_ReadCounter(i), 0);
        TEST_ASSERT_EQUAL (BLADE_ReadCalibrated(i), 0);
    }
}

/* IRDA_ConfirmID (AC) */

void IRDA_ConfirmID_Init (uint8_t max, uint16_t id)
{
    TestInit_irdaStable(id); 
    uint8_t i;
    for (i=1; i<=max; i++)
    {  
        uint8_t addr = channel2address(i);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
        IRDA_UpdateTable(addr, i, getId(i), BLADE_ReadState(i), response);
    }
}

void test_IRDA_ConfirmID_irdaStable_ResultOk()
{
    uint8_t actual[MAX_NUM_OF_BYTES];
    IRDA_ConfirmID_Init(5, 0xABCD);
    
    uint8_t i = 3;
    uint8_t expected[] = "\nAC3: Calibrated (ABCD)";

    IRDA_ConfirmID(i, i, getId(i),  BLADE_ReadState(i), actual);
    TEST_ASSERT_EQUAL_STRING(expected, actual);
}

void test_IRDA_ConfirmID_irdaStable_ResultFail()
{
    uint8_t actual[MAX_NUM_OF_BYTES];
    uint8_t expected[MAX_NUM_OF_BYTES];
    IRDA_ConfirmID_Init(5, 0xABCD);
        
    uint8_t i;
    for (i=1; i<=5; i++)
    {    
      uint8_t addr = channel2address(i);
      sprintf(expected, CONSOLE_ERR_ACK_INVALID_ID, addr, 0x1234, 0xABCD);
      IRDA_ConfirmID(addr, i, 0x1234, BLADE_ReadState(i), actual);
      TEST_ASSERT_EQUAL_STRING(expected, actual);  
    }
}

void test_IRDA_ConfirmID_irdaStable_SendMS()
{
    uint8_t actual[MAX_NUM_OF_BYTES];
    uint8_t message[MAX_NUM_OF_BYTES];
    uint8_t expected[MAX_NUM_OF_BYTES];
    IRDA_ConfirmID_Init(5, 0xABCD);
    
    uint8_t i = 3;
    Scheduled_Channel[i-1] = 1;
    uint8_t addr = channel2address(i);
    sprintf(message, CONSOLE_IR_REQ_MS, addr, BLADE_ReadID(i), CONSOLE_EXAMPLE_COMMAND);
    sprintf(expected, "\nAC2: %s 931A", message);
    IRDA_ConfirmID(addr, i, getId(i), BLADE_ReadID(i), actual);
    TEST_ASSERT_EQUAL_STRING(expected, actual);

    for (i=1; i<=5; i++)
    {
      Scheduled_Channel[i-1] = 1;
      uint8_t addr = channel2address(i);
      IRDA_ConfirmID(addr, i, getId(i), BLADE_ReadID(i), actual);
      TEST_ASSERT_EQUAL(Scheduled_Channel[i-1], 0);
    }
}

void test_IRDA_ConfirmID_messageWaiting_SendMR()
{
    uint8_t actual[MAX_NUM_OF_BYTES];
    IRDA_ConfirmID_Init(5, 0xABCD);

    IRDA_STATUS_BYTE status;
    status.irdaStatus = 0x03;
    uint8_t i = 3;    
    uint8_t expected[] = "\nAC2: $MR2 ABCD C244";
    uint8_t addr = channel2address(i);
    IRDA_ConfirmID(addr, i, getId(i), status, actual);
    TEST_ASSERT_EQUAL_STRING(expected, actual);

    // MR more priority than MS
    for (i=1; i<=5; i++)
    {
      Scheduled_Channel[i-1] = 1;    
      uint8_t addr = channel2address(i);
      IRDA_ConfirmID(addr, i, getId(i), status, actual);
      TEST_ASSERT_EQUAL(Scheduled_Channel[i-1], 1);
    }
}

/* Uart0Message AT and AC */

void test_IRDA_Uart0Message_AT_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "\nTraining channel 2";

   BLADE_ResetAll();
   
   init_Command (5, CONSOLE_IR_REPLY_START); 
   command.argv[1] = "AT2"; 
   command.argv[2] = "ABCD";  
   command.argv[3] = "1";
   command.argv[4] = "DEAD";  

   IRDA_Uart0Message(&command, 2, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);     
}

void test_IRDA_Uart0Message_AC_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "\nAC0: Calibrated (1234)";
   uint8_t device = address2channel(0);
   
   BLADE_Uncalibrate (device);
   BLADE_WriteID(device, 0x1234);

   init_Command (5, CONSOLE_IR_REPLY_START);
   command.argv[1] = "AC0";
   command.argv[2] = "1234";   
   command.argv[3] = "1";
   command.argv[4] = "DEAD";   

   IRDA_Uart0Message(&command, 1, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);     
}

void test_IRDA_Uart0Message_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "\n* NACK";

   init_Command (1, CONSOLE_IR_REPLY_START);   
   IRDA_Uart0Message(&command, 1, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);

   init_Command (2, CONSOLE_IR_REPLY_START);   
   IRDA_Uart0Message(&command, 2, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual); 

   init_Command (3, CONSOLE_IR_REPLY_START);   
   IRDA_Uart0Message(&command, 3, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual); 

   init_Command (4, CONSOLE_IR_REPLY_START);   
   IRDA_Uart0Message(&command, 4, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual); 
}

/* Uart0Message AS and AR */

void test_IRDA_Uart0Message_AS_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "\n* AS1 1234 1";

   init_Command (5, CONSOLE_IR_REPLY_START);
   command.argv[1] = "AS1";
   command.argv[2] = "1234";   
   command.argv[3] = "1";
   command.argv[4] = "DEAD";

   IRDA_Uart0Message(&command, 1, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_IRDA_Uart0Message_AR_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "\n* AR3 1234 1 * 123 MST SMM OK ABCD 71A6";

   init_Command (6, CONSOLE_IR_REPLY_START);
   command.argv[1] = "AR3";
   command.argv[2] = "1234";   
   command.argv[3] = "1";
   command.argv[4] = CONSOLE_EXAMPLE_ANSWER;      // Multiple
   command.argv[5] = "BEEF";                      // CRC

   IRDA_Uart0Message(&command, 1, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);     
}

/* BeaconPrecheck */

void test_IRDA_BeaconPrecheck_OldId()
{
    uint8_t actual[MAX_NUM_OF_BYTES];
    uint8_t expected[MAX_NUM_OF_BYTES];

    TestInit_irdaStable(0xABCD);    
    uint8_t i;
    for (i=1; i<=5; i++)
    {
      uint8_t addr = channel2address(i);
      sprintf (expected, CONSOLE_ERR_ACK_INVALID_ID, addr, 0x1234, 0xABCD);
      IRDA_UpdateTable (addr, i, 0x1234, BLADE_ReadState(i), actual);
      TEST_ASSERT_EQUAL_STRING(expected, actual);
    }
}

void test_IRDA_BeaconPrecheck_IdFFFF()
{
    uint8_t actual[MAX_NUM_OF_BYTES];
    uint8_t expected[MAX_NUM_OF_BYTES];

    TestInit_irdaStable(0xABCD);    
    uint8_t i;
    for (i=1; i<=5; i++)
    {
      uint8_t addr = channel2address(i);
      sprintf (expected, CONSOLE_ERR_ACK_ID_FFFF, addr, 0xFFFF);
      IRDA_UpdateTable (addr, i, 0xFFFF, BLADE_ReadState(i), actual);
      TEST_ASSERT_EQUAL_STRING(expected, actual);
    }
}

void test_IRDA_BeaconPrecheck_NullStatus()
{
    uint8_t actual[MAX_NUM_OF_BYTES];
    uint8_t expected[MAX_NUM_OF_BYTES];

    IRDA_STATUS_BYTE status;
    status.irdaStatus = 0x00;    
    TestInit_irdaStable(0xABCD);

    uint8_t i;
    for (i=1; i<=5; i++)
    {
      uint8_t addr = channel2address(i);
      sprintf (expected, CONSOLE_ERR_ACK_INVALID_STATUS, addr, 0);
      IRDA_ConfirmID (addr, i, getId(i), status, actual);
      TEST_ASSERT_EQUAL_STRING(expected, actual);      
    }    
}

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning

    UNITY_BEGIN();

    printf("\n\r");
    printf("IRDA_Uart0Message: ACx, ATx, ASx, ARx \r\n");
    RUN_TEST(test_IRDA_Uart0Message_AT_ResultOk);
    RUN_TEST(test_IRDA_Uart0Message_AC_ResultOk);
    RUN_TEST(test_IRDA_Uart0Message_AS_ResultOk);
    RUN_TEST(test_IRDA_Uart0Message_AR_ResultOk);
    RUN_TEST(test_IRDA_Uart0Message_MissingArgs);
    printf("BeaconPrecheck \n\r");
    RUN_TEST(test_IRDA_BeaconPrecheck_OldId);
    RUN_TEST(test_IRDA_BeaconPrecheck_IdFFFF);
    RUN_TEST(test_IRDA_BeaconPrecheck_NullStatus);
    printf("IRDA_UpdateTable \n\r");
    RUN_TEST(test_IRDA_UpdateTable_TwoIterations);
    RUN_TEST(test_IRDA_UpdateTable_ThreeIterations);
    RUN_TEST(test_IRDA_UpdateTable_FourIterationsAndCalibrated);
    RUN_TEST(test_IRDA_UpdateTable_FourIterationsAndUncalibrate);
    printf("IRDA_ConfirmID \n\r");
    RUN_TEST(test_IRDA_ConfirmID_irdaStable_ResultFail);
    RUN_TEST(test_IRDA_ConfirmID_irdaStable_ResultOk);
    RUN_TEST(test_IRDA_ConfirmID_irdaStable_SendMS);
    RUN_TEST(test_IRDA_ConfirmID_messageWaiting_SendMR);
    return UNITY_END();
}