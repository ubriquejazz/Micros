#include <stdio.h>
#include <string.h>
#include "unity.h"

void test_AppendCrc_Example_Command(void)
{
  char expected[] = CONSOLE_EXAMPLE_COMMAND;  // Ending in C08B
  char message[MAX_NUM_OF_BYTES];

  sprintf(message, "! %d SMM MST BLADE_CTRL READ_MASTER_VER", 123);
  uint16_t crc_value = AppendCrc(message);
  TEST_ASSERT_EQUAL(0xB80C, crc_value);
  TEST_ASSERT_EQUAL_STRING(expected, message);
}

void test_AppendCrc_Example_Answer(void)
{
  char expected[] = CONSOLE_EXAMPLE_ANSWER; // Ending in 71A6
  char message[MAX_NUM_OF_BYTES];

  sprintf(message, "* %d MST SMM OK ABCD", 123);
  uint16_t crc_value = AppendCrc(message);
  TEST_ASSERT_EQUAL(crc_value, 0x6A17);
  TEST_ASSERT_EQUAL_STRING(message, expected);
}

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning
    
    UNITY_BEGIN();
    printf("CRC Gral Functions\n\r");

    RUN_TEST(test_AppendCrc_Example_Command);
    RUN_TEST(test_AppendCrc_Example_Answer);

    return UNITY_END();
}
