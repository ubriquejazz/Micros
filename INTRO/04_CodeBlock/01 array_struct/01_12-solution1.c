#include <stdio.h>

typedef struct {
	int month;
	int day;
	int year;
} Date;

typedef struct 
{
	char 	name[32];
	Date	tookoffice;
} Person;

void show(Person p[], int len)
{
	int x;
	puts("Presidents:");
	for(x=0; x<len; x++)
	{
		printf("President %s took office on %d/%d/%d\n", p[x].name,
				p[x].tookoffice.month,
				p[x].tookoffice.day,
				p[x].tookoffice.year);
	}
}

int main()
{
	Person presidents[4] = {
		{ "George Washington", { 4, 30, 1789 } },
		{ "Thomas Jefferson", { 3, 4, 1801 } },
		{ "Abraham Lincoln", { 3, 4, 1861 } },
		{ "Theodore Roosevelt", { 9, 14, 1901 } }
	};
	Person temp;

	/* show them */
	show(presidents, 4);
	/* swap 2 and 3 */
	printf("Swapping...");
	temp = presidents[1];
	presidents[1] = presidents[2];
	presidents[2] = temp;
	printf("done!\n");
	/* show again */
	show(presidents, 4);

	return(0);
}

