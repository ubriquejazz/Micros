#ifndef _LIST_H
#define _LIST_H

/*!\name      LIST.h
 *
 * \brief     Blah, blah
 *
 * \author    Juan Gago
 *
 */

#include <stdio.h>
#include <stdint.h>

typedef struct LINKEDLIST LinkedList;
typedef struct NODE Node;

// A Linked List.
struct LINKEDLIST {
  Node *root;
  Node *end;
  int length;
};

// Node in a Linked List.
struct NODE {
  int data;
  Node *prev;
  Node *next;
};

void bubble_sort (uint8_t* data);

// Node stuff.
Node * new_node		(int);
void print_node_buff(Node *, uint8_t *);
void print_node		(Node *);
void destroy_node   (Node *);
void destroy_nodes  (Node *);

// LinkedList specific
LinkedList * new_list	(int);
LinkedList * push		(LinkedList *, int);
LinkedList * insert		(LinkedList *, int);
LinkedList * list_remove(LinkedList *, int);

int pop				(LinkedList *);
int head			(LinkedList *);
int contains		(LinkedList *, int);
int length			(LinkedList *);
void print_list		(LinkedList *);
void destroy_list	(LinkedList *);

#endif