#include <stdio.h>
#include "list.h"

void test_remove();
void test_contains();
void test_length();
void test_pop();
void test_head();

int main() {
  printf("\nIntegration tests!\n");
  printf("------------------\n\n");
  test_remove();
  test_contains();
  test_length();
  test_pop();
  test_head();
  printf("Tests complete.\n");
  return 0;
}

void test_remove() {
  LinkedList *list = NULL;
  
  printf("Testing remove()...\n");
  
  // Remove the only element.
  list = new_list(42);
  printf("Before...\n");
  print_list(list);
  list = list_remove(list, 42);
  printf("After...\n");
  print_list(list);

  // Remove the end.
  list = new_list(42);
  list = push(list, 1);
  list = push(list, 2);
  printf("Before...\n");
  print_list(list);
  list = list_remove(list, 2);
  printf("After...\n");
  print_list(list);

  // Remove a middle one.
  list = push(list, 5);
  list = push(list, 999);
  printf("Before...\n");
  print_list(list);
  list = list_remove(list, 1);
  printf("After...\n");
  print_list(list);

  printf("\n");
}

void test_contains() {
  LinkedList *list = NULL;
  int result;

  printf("Testing contains()...\n");

  list = new_list(42);
  print_list(list);

  // Definitely in there.
  result = contains(list, 42);
  printf("list contains 42: %d\n", result);

  // Definitely not in there.
  result = contains(list, 999);
  printf("list contains 999: %d\n", result);

  printf("\n");
}

void test_length() {
  LinkedList *list = NULL;
  int result;

  printf("Testing length()...\n");

  list = new_list(42);
  print_list(list);

  result = length(list);
  printf("list length %d\n", result);

  list = push(list, 1);
  result = length(list);
  printf("after push - length %d\n", result);

  list = insert(list, 55);
  result = length(list);
  printf("after insert - length %d\n", result);

  list = list_remove(list, 1);
  result = length(list);
  printf("after remove - length %d\n", result);

  printf("\n");
}

void test_pop_helper(LinkedList *list) {
  int result;
  printf("Unpopped: ");
  print_list(list);
  result = pop(list);
  printf("Got: %d\n", result);
  printf("Popped: ");
  print_list(list);
}

void test_pop() {
  LinkedList *list = NULL;

  printf("Testing test_pop()...\n");

  // Pop only element.
  list = new_list(42);
  test_pop_helper(list);

  // Pop off normal list.
  list = new_list(42);
  list = push(list, 5);
  list = push(list, 9);
  test_pop_helper(list);

  // Pop off empty list.
  list = new_list(42);
  list = list_remove(list, 42);
  test_pop_helper(list);

  printf("\n");
}

void test_head_helper(LinkedList *list) {
  int result;
  printf("Before: ");
  print_list(list);
  result = head(list);
  printf("Got: %d\n", result);
  printf("After: ");
  print_list(list);
}

void test_head() {
  LinkedList *list = NULL;

  printf("Testing head()...\n");

  // Get only element.
  list = new_list(42);
  test_head_helper(list);

  // Get head from normal list.
  list = new_list(42);
  list = insert(list, 11);
  list = insert(list, 66);
  test_head_helper(list);

  // Get head from empty list.
  list = new_list(42);
  list = list_remove(list, 42);
  test_head_helper(list);

  printf("\n");
}




