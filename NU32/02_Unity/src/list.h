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

typedef struct node {
    int val;
    struct node* next;

} NODE_T;

void bubble_sort (uint8_t* data);
void insert_by_index(NODE_T ** head, int n);
int remove_by_index(NODE_T ** head, int n) ;
void print_list (NODE_T * head, uint8_t* result) ;

#endif