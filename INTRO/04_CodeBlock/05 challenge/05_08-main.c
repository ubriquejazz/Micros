#include "05_08-horserace.h"

int main()
{
	Pony racehorse[RECORDS];
	int winner;

	HORSE_Initialize(racehorse);
	HORSE_Run_Race(racehorse);
	winner = HORSE_Show_Results(racehorse);
	printf("The winner is %s!\n",racehorse[winner].name);
	return(0);
}
