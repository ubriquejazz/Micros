#include <stdio.h>
#include "list.h"

uint8_t result[10] = "World!";

int main()
{
	NODE_T myNode;
	//insert_by_index(&myNode, 0);
	//print_list(&myNode, result);
	printf("Hello %s!\r\n", result);
	return(0);
}

