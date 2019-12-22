#include <stdio.h>

typedef struct node {
    int val;
    struct node* next;

} NODE_T;

void insert_by_index(NODE_T ** head, int n) 
{
    NODE_T* new_node;
    NODE_T* prev_node = *head;
    new_node = (NODE_T*)malloc(sizeof(NODE_T));
    new_node->val = n;

    // Adding an item to the beginning of the list
    if ( *head == NULL || (prev_node->val >= n) )
    {
        new_node->next = *head;
        *head = new_node;
    }
    else
    {
        while ( prev_node->next != NULL && (prev_node->next->val < n) )
        {
            prev_node = prev_node->next;
        }
        // Founded the correct place
        new_node->next = prev_node->next;
        prev_node->next = new_node;
    }
}

int remove_by_index(NODE_T ** head, int n) 
{
    int retval = -1;
    NODE_T * current = *head;
    NODE_T * prev_node = NULL;

    if (*head != NULL)
    {
        while ( current != NULL && (current->val < n) )
        {
            prev_node = current;
            current = current->next;
        }
        
        if ( current != NULL && (current->val == n) )
        {
            // Founded the correct place
            if (current == *head)
            {
                *head = current->next;
            }
            else
            {
                prev_node->next = current->next;
            }
            retval = 1;
            free(current);
        }
        else
        {
            // Nothing found
            retval = 0;
        }
    }
    return retval;
}

void print_list (NODE_T * head, uint8_t* result) 
{
    NODE_T * current = head;
    while (current != NULL) 
    {
        sprintf(result, "%d %s\n", current->val);
        current = current->next;
    }
}

void bubble_sort (uint8_t* data)
{
    uint8_t temp;
    int i, j;
    int size = strlen(data);
    
    for (i=0; i<size; i++)
    {
        for (j=0; j<(size-1); j++)
        {
            temp = data[j];
            data[j] = data[j+1];
            data[j+1] = temp;
        }
    }
}