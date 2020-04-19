#ifndef _TEST_COMMON_H
#define _TEST_COMMON_H

/*!\name      test_common.h
 *
 * \brief     API for PIN testing
 *
 * \author    Juan Gago
 *
 */

extern bool TestError[];

int test_init (void);
int test_run (int);

#endif /* _TEST_COMMON_H */