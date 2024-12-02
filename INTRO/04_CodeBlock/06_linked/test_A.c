#include <stdio.h>
#include <string.h>
#include "unity.h"

void test_CreateNode(void) {
	uint8_t expected[] = "1 - End\n";
	uint8_t message[9];
	Node* ptr;
	ptr = new_node(1);
	print_node_buff (ptr, message);
	TEST_ASSERT_EQUAL_STRING(expected, message);
}
void test_DestroyNode(void) {
	Node* ptr;
	ptr = new_node(1);
	destroy_node(ptr);
	TEST_ASSERT_NOT_EQUAL(NULL, ptr);
}

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning
    
    UNITY_BEGIN();
    printf("Test A\n\r");
    RUN_TEST(test_CreateNode);
    RUN_TEST(test_DestroyNode);
    return UNITY_END();
}
