/*!\name      LIST.h
 *
 * \brief     Blah, blah
 *
 * \author    Juan Gago
 *
 */

#include "list.h"

// NODE FUNCTIONS

Node * new_node(int data) {
  // Creates a new Node.
  Node *node = NULL;
  node = malloc(sizeof(Node));
  
  if(node != NULL) {
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
  }
  return node;
}

void print_node(Node *node) {
  // Prints a Node all pretty-like.
  if(node != NULL) {
    printf("%d - ", node->data);
    if(node->next == NULL)
        printf("End\n");
  }
}

void print_node_buff(Node *node, uint8_t* result) {
  // Prints a Node all pretty-like.
  if(node != NULL) {
    sprintf(result, "%d - ", node->data);
    if(node->next == NULL)
      strcat(result, "End\n");
  }
}

void destroy_node(Node *node) {
  // Destroys the given Node. With rage.
  free(node);
  node = NULL;
}

void destroy_nodes(Node *node) {
  // Recursively frees all the Nodes.
  if(node != NULL) {
    if(node->next == NULL)
      destroy_node(node);
    else {
      destroy_nodes(node->next);
      destroy_node(node);
    }
  }
}

// LINKED LIST FUNCTIONS

LinkedList * new_list(int data) {
  // Creates a new Linked List.
  LinkedList *list = NULL;
  list = malloc(sizeof(LinkedList));
  if(list != NULL) {
    list->root = new_node(data);
    list->end  = list->root;
    list->length = 1;
  }
  return list;
}

LinkedList * push(LinkedList *list, int data) {
  // Adds new data to the end of the list.
  Node *node = NULL;
  if(list != NULL) {
    node = new_node(data);
    if(node != NULL) {
      if(list->root == NULL)  // Pushing into an empty list.
        list->root = node;
      else {
        list->end->next = node;
        node->prev = list->end;
      }
      list->end = node;
      list->length += 1;
    }
  }
  return list;
}

LinkedList * insert(LinkedList *list, int data) {
  // Adds new data to the front of the list.
  Node *node = NULL;
  if(list != NULL) {
    node = new_node(data);
    if(node != NULL) {
      node->next = list->root;
      list->root->prev = node;
      list->root = node;
      list->length += 1;
    }
  }
  return list;
}

int list_remove_helper(LinkedList *list, Node *curr)
{
  if(curr == list->root)  // Removing the root.
  {  
    list->root = list->root->next;
    if(list->length == 0)
      list->end = NULL;
    else
      list->root->prev = NULL;
  }
  else if(curr->next == NULL) // Removing last node.
  {  
    curr->prev->next = NULL;
    list->end = curr->prev;
  }
  else  // A normal node.
    curr->prev->next = curr->next;
  return 0;
}

LinkedList * list_remove(LinkedList *list, int data) {
  // Removes a given piece of data from the list.
  Node *curr;
  if(list != NULL) {
    curr = list->root;
    while(curr != NULL) {
      if(curr->data == data) {
        list->length -= 1;
        list_remove_helper(list, curr);
        destroy_node(curr);
        break;
      }
      curr = curr->next;
    }
  }
  return list;
}

int pop(LinkedList *list) {
  // Yields the last item in the list.
  int result = 0;
  Node *temp;
  if(list != NULL && list->length > 0) {
    temp = list->end;
    result = temp->data;
    list->length -= 1;
    if(list->length == 0) {  // We emptied the list.
      list->root = NULL;
      list->end = NULL;
    }
    else {  // Still other nodes left.
      temp->prev->next = NULL;
      list->end = temp->prev;
    }
    destroy_node(temp);
  }
  else {
    perror("ERROR: Empty List passed!");
  }
  return result;
}

int head(LinkedList *list) {
  // Yields the first item in the list.
  int result = 0;
  Node *temp;
  
  if(list != NULL && list->length > 0) {
    temp = list->root;
    result = temp->data;
    list->root = temp->next;
    list->length -= 1;
    if(list->length > 0)
      list->root->prev = NULL;
    else
      list->end = NULL;

    destroy_node(temp);
  }
  else {
    perror("ERROR: Empty List passed!");
  }
  return result;
}

int contains(LinkedList *list, int data) {
  // Checks if a list contains a given piece of data.
  Node *curr;
  int result = 0;  // Assume failure.

  if(list != NULL) {
    curr = list->root;
    while(curr != NULL) {
      if(curr->data == data) {
        result = 1;
        break;
      }
      curr = curr->next;
    }
  }
  return result;
}

int length(LinkedList *list) {
  // Yields the length of the list.
  int result = -1;
  if(list != NULL)
    result = list->length;
  return result;
}

void print_list(LinkedList *list) {
  // Prints a given Linked List.
  Node *curr;

  if(list != NULL) {
    curr = list->root;
    if(curr == NULL)
      printf("Empty\n");
    while(curr != NULL) {
      print_node(curr);
      curr = curr->next;
    }
  }
}

void destroy_list(LinkedList *list) {
  // Emancipates a Linked List from its memory-bound slavery.
  if(list != NULL) {
    destroy_nodes(list->root);
    free(list);
  }
}

// BUBBLE SORT

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

/*
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
*/