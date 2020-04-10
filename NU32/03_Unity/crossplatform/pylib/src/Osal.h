#ifndef _Osal_H_
#define _Osal_H_

/*!\name      Osal.c
 *
 * \brief     Abstraction layer for portable code.
 *
 */

#include <stdint.h>
#include <stdbool.h>

#ifdef LINUX_BUILD
   #include <pthread.h>
   typedef pthread_mutex_t MrtsMutex_t;
   typedef pthread_mutexattr_t MrtsMutexAttr_t;
#else
   /* Structures for if we have nothing better to use */
   typedef struct MrtsMutexAttr_t
   {
      uint32_t someBits;
   } MrtsMutexAttr_t;

   typedef struct MrtsMutex_t
   {
     bool isLocked;
     MrtsMutexAttr_t attributes;
   } MrtsMutex_t;


#endif


#ifdef DPRINTF_ENABLED
   #include <stdio.h>
   #define DPRINTF(...) printf(__VA_ARGS__)
#else
   #define DPRINTF(...) //printf(__VA_ARGS__)
#endif

int32_t MrtsMutexCreate( MrtsMutex_t* mutex, MrtsMutexAttr_t* attr );
int32_t MrtsMutexLock( MrtsMutex_t* mutex );
int32_t MrtsMutexTryLock( MrtsMutex_t* mutex );
int32_t MrtsMutexUnlock( MrtsMutex_t* mutex );
int32_t MrtsMutexDestroy( MrtsMutex_t* mutex );


#endif /* _Osal_H_ */
