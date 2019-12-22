#include <stdio.h>
#include <string.h>
#include "unity.h"

#define CONSOLE_IR_BCX_OK "* AC1 1234 1 05C2\r"
#define CONSOLE_IR_BTX_OK "* AT4 1234 1 0F3D\r";  
#define CONSOLE_IR_MSX_OK "* AS2 FFFF 1 65AD\r"

COMMAND_DATA command;

extern IRDA_STATUS_BYTE status;

void init_Command(int argc, const char* name)
{
   command.argv[0]= name; 
   command.argc = argc;
}

/****** IR Receive Bx ******/

void test_DUMMY_Receive_BC_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = CONSOLE_IR_BCX_OK;  

   init_Command(3, "$BC1");
   command.argv[1] = "1234";
   command.argv[2] = "DEAD";    // CRC
   DUMMY_Receive_BC (&command, actual);

   DUMMY_UpdateStatus();
   //sprintf (expected, CONSOLE_IR_ACK_BC, 1, status.Byte, fakedID); 
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_DUMMY_Receive_BT_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = CONSOLE_IR_BTX_OK;
  
   init_Command(2, "$BT4");     // 
   command.argv[1] = "DEAD";    // CRC
   DUMMY_Receive_BT (&command, actual);
 
   DUMMY_UpdateStatus();
   //sprintf (expected, CONSOLE_IR_ACK_BT, 1, status.Byte, fakedID); 
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_DUMMY_Receive_Bx_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = CONSOLE_IR_NACK;

   init_Command  (1, "$BC");   
   DUMMY_Receive_BT (&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);  

   init_Command  (2, "$BC");   
   DUMMY_Receive_BC (&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);  

   init_Command  (1, "$BT");   
   DUMMY_Receive_BT (&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

/****** IR Receive Mx ******/

void test_DUMMY_Receive_MS_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = CONSOLE_IR_MSX_OK;  
   uint8_t Irda_FakedCommand[] = CONSOLE_EXAMPLE_COMMAND;

   init_Command(4, "$MS2");
   command.argv[1] = "1234";
   command.argv[2] = Irda_FakedCommand;   // Multiple!!!
   command.argv[3] = "DEAD";              // CRC
   DUMMY_Receive_MS (&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_DUMMY_Receive_MR_ResultOk(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = "* AR0 1234 1 * 123 MST SMM OK ABCD 71A6 F357\r";  

   init_Command(3, "$MR0");
   command.argv[1] = "1234";
   command.argv[2] = "DEAD";              // CRC
   DUMMY_Receive_MR (&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

void test_DUMMY_Receive_Mx_MissingArgs(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t expected[] = CONSOLE_IR_NACK;

   init_Command  (1, "$MS");   
   DUMMY_Receive_MS (&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);  

   init_Command  (2, "$MS");   
   DUMMY_Receive_MS (&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);  

   init_Command  (1, "$MR");   
   DUMMY_Receive_MR (&command, actual);
   TEST_ASSERT_EQUAL_STRING(expected, actual);   
}

/****** IR Receive Mx ******/

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning
    
    UNITY_BEGIN();
    
    printf("\n\r");
    printf("DUMMY_Receive from IrDA Pic (BC, BT)\r\n");
    RUN_TEST(test_DUMMY_Receive_BC_ResultOk);
    RUN_TEST(test_DUMMY_Receive_BT_ResultOk);
    RUN_TEST(test_DUMMY_Receive_Bx_MissingArgs);
    printf("DUMMY_Receive from IrDA Pic (MS, MR)\r\n");
    RUN_TEST(test_DUMMY_Receive_MS_ResultOk);
    RUN_TEST(test_DUMMY_Receive_MR_ResultOk);
    RUN_TEST(test_DUMMY_Receive_Mx_MissingArgs);

    return UNITY_END();
}
