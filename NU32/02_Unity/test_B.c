#include <stdio.h>
#include <string.h>
#include "unity.h"

void test_new_list() {
  LinkedList *list = NULL;
  list = new_list(42);
  TEST_ASSERT_NOT_EQUAL(NULL, list);
}
void test_push() {
  LinkedList *list = NULL;
  list = new_list(42);
  list = push(list, 49);
  TEST_ASSERT_NOT_EQUAL(NULL, list);
}
void test_insert() {
  LinkedList *list = NULL;
  list = new_list(42);
  list = insert(list, 11);
  TEST_ASSERT_NOT_EQUAL(NULL, list);
}

int main(int argc, char** argv)
{
   (void) argc; // Cast to void to avoid warning
   (void) argv; // Cast to void to avoid warning
    
    UNITY_BEGIN();
    printf("Test B\n\r");
    RUN_TEST(test_new_list);
    RUN_TEST(test_insert);
    RUN_TEST(test_push);
    return UNITY_END();
}
