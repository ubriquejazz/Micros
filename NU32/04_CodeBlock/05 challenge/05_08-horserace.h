#ifndef _HORSERACE_H
#define _HORSERACE_H

/*!\name      horserace.h
 *
 * \brief     Blah, blah
 *
 * \author    Juan Gago
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RECORDS 5
#define NAME_SIZE 20
#define RACE_LENGTH 9

typedef struct {
	char *name;
	int distance[RACE_LENGTH];
	int first[RACE_LENGTH];
} Pony;

/* must include prototypes from other modules */
void HORSE_Initialize	(Pony horse[]);
void HORSE_Run_Race		(Pony horse[]);
int  HORSE_Show_Results	(Pony horse[]);

#endif