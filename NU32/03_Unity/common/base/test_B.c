#include <stdio.h>
#include <string.h>
#include "unity.h"

COMMAND_DATA command;

void init_Command(int argc, const char* name)
{
   command.seq_no   = "123";
   command.src_addr = "USB";
   command.dst_addr = "MST";
   command.checksum = 0;
   command.argv[0]  = name; 
   command.argc     = argc;
}

void test_populate_command_HeaderOk(void)
{
   uint8_t buffer[] = "! 789 SMM IRA HELP";
   uint8_t actual;

   actual = validate_header(buffer, 14);
   TEST_ASSERT_EQUAL(1, actual);

   init_Command(1, "HELP");
   actual = populate_command (buffer, &command);
   TEST_ASSERT_EQUAL(1, actual);
}
/* BT4 */

void test_CheckCrc_Beacon_BT4(void)
{
  uint8_t  rx_buf [MAX_NUM_OF_BYTES] = "$BT4 B7D6";
  uint32_t rx_count = 10;
  uint32_t crc_start = rx_count-5;
  uint16_t crc_received = CalculateCrc(rx_buf, crc_start);
  TEST_ASSERT_EQUAL(crc_received, 0x6D7B);  
}

void test_AppendCrc_Beacon_BT4(void)
{
  char message[MAX_NUM_OF_BYTES] = "$BT4";

  uint16_t value = hexa2decimal("6D7B");
  TEST_ASSERT_EQUAL(value, 0x6D7B);

  uint16_t crc = AppendCrc(message);
  TEST_ASSERT_EQUAL(crc, 0x6D7B);
  TEST_ASSERT_EQUAL_STRING (message, "$BT4 B7D6");
}

void test_crc_update_BT4_ResultOk()
{
  char message[MAX_NUM_OF_BYTES] = "$BT4";
  char expected[] = "$BT4 B7D6";

  init_Command(2, "$BT4");

  command.argv[1] = "B7D6";
  crc_update(&command);
  TEST_ASSERT_EQUAL(command.checksum, 0x6D7B);
}

void test_crc_update_BT4_ResultFail()
{
  char message[MAX_NUM_OF_BYTES] = "$BT4";
  char expected[] = "$BT4 B7D6";

  init_Command(2, "$BT4");

  command.argv[1] = "123";
  crc_update(&command);
  TEST_ASSERT_EQUAL(command.checksum, (uint16_t)-1);

  command.argv[1] = "1234";
  crc_update(&command);
  TEST_ASSERT_EQUAL(command.checksum, 0x4321);
}

/* MS2 */

void test_crc_update_MS2_ResultFail(void)
{
   uint8_t actual[MAX_NUM_OF_BYTES];
   uint8_t buffer[] = "$MS2 1234 * ABCD";   

   init_Command(4, "$MS2");
   command.argv[1] = "1234";
   command.argv[2] = CONSOLE_EXAMPLE_COMMAND;
   command.argv[3] = "ABCD";
 
   //populate_command (buffer, &command);   
   //TEST_ASSERT_EQUAL(command.argc, 4);s

// This assert fails beacuse populate_command()
// shouldn't run with these beacons (Dummy Pic)
// See UART_CrcUpdate() for full-duplex mode and MSx

   crc_update(&command);
   TEST_ASSERT_EQUAL(command.checksum, 0xDCBA);
}



int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning
    
    UNITY_BEGIN();
    printf("CRC in BT and MS beaconsm\n\r");

    RUN_TEST(test_populate_command_HeaderOk);

    RUN_TEST(test_CheckCrc_Beacon_BT4);    
    RUN_TEST(test_AppendCrc_Beacon_BT4);
    RUN_TEST(test_crc_update_BT4_ResultOk);
    RUN_TEST(test_crc_update_BT4_ResultFail);
    RUN_TEST(test_crc_update_MS2_ResultFail);
    
    return UNITY_END();
}
