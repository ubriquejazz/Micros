/*!\name      Osal.c
 *
 * \brief     Abstraction layer for portable code.
 *
 */

#include <stdio.h>
#include <stdint.h>

#ifdef LINUX_BUILD
   #include <errno.h>
   #include <pthread.h>
#endif

#include "Osal.h"
#include "mrtsErrors.h"


#ifdef LINUX_BUILD

int32_t ConvertLinuxReturnToMrtsError(int linuxRet)
{
   int32_t retVal = merUnknownError;

   switch (linuxRet)
   {
   case 0:
      retVal = merNoError;
      break;

   case EBUSY:
      retVal = merBusy;
      break;

   case EDEADLK:
      retVal = merDeadlock;
      break;

   case EAGAIN:
      retVal = merMutexMaxLocksExceeded;
      break;

   case EINVAL:
      retVal = merInvalidArgument;
      break;

   default:
      retVal = merUnknownError;
   }

   return retVal;
}

/*!\name      MrtsMutexCreate
 *
 * \brief     Attempt to create a mutex.
 *
 * \returns   mrtsError (negative fail)
 */
int32_t MrtsMutexCreate( MrtsMutex_t* mutex, MrtsMutexAttr_t* attr )
{
   int32_t retVal = merNoError;
   (void ) attr;

   if (0 != pthread_mutex_init(mutex, NULL))
   {
      retVal = merMutexInitFailed;
   }

   return retVal;
}
#else
int32_t MrtsMutexCreate( MrtsMutex_t* mutex, MrtsMutexAttr_t* attr )
{
   int32_t retVal = merNoError;

   if (NULL == mutex )
   {
      return merNullPointer;
   }
   mutex->isLocked = false;
   if (NULL == attr)
   {
      /* Not really an error but might need some handler */
   }
   else
   {
      mutex->attributes = *attr;
   }

   return retVal;
}
#endif

/*!\name      MrtsMutexLock
 *
 * \brief     Attempt to lock previously created mutex. If it is not
 *            immediately available block until it has been unlocked by its
 *            other user.
 *
 * \returns   mrtsError (negative fail)
 */
#ifdef LINUX_BUILD
int32_t MrtsMutexLock( MrtsMutex_t* mutex )
{
   int32_t retVal = merNullPointer;

   if (mutex)
   {
      int response = pthread_mutex_lock(mutex);
      retVal = ConvertLinuxReturnToMrtsError(response);
   }

   return retVal;
}

#else
int32_t MrtsMutexLock( MrtsMutex_t* mutex )
{
   int32_t retVal = merNoError;

   /*printf("MrtsMutexLock ");*/
   while (mutex->isLocked)
   {
      /* We may need to throw an error here or flash an LED */
   }

   /*printf("locked\n\r ");*/
   mutex->isLocked = true;

   return retVal;
}
#endif


/*!\name      MrtsMutexTryLock
 *
 * \brief     Attempt to lock previously created mutex, fail immediately if
 *            this can not be done.
 *
 * \returns   mrtsError (negative fail)
 */
#ifdef LINUX_BUILD
int32_t MrtsMutexTryLock( MrtsMutex_t* mutex )
{
   int32_t retVal = merNullPointer;
   if (mutex)
   {
      int response = pthread_mutex_trylock(mutex);
      retVal = ConvertLinuxReturnToMrtsError(response);
   }

   return retVal;
}
#else
int32_t MrtsMutexTryLock( MrtsMutex_t* mutex )
{
   int32_t retVal = merNoError;

   /*printf("MrtsMutexLock ");*/
   while (mutex->isLocked)
   {
      volatile static int i=0;
      
      /* We may need to throw an error here or flash an LED */
      i++;
      
   }

   /*printf("locked\n\r ");*/
   mutex->isLocked = true;

   return retVal;
}
#endif

/*!\name      MrtsMutexUnlock
 *
 * \brief     Unlock previously created mutex/
 *
 * \returns   mrtsError (negative fail)
 */
#ifdef LINUX_BUILD
int32_t MrtsMutexUnlock( MrtsMutex_t* mutex )
{
   int32_t retVal = merNullPointer;

   if (mutex)
   {
      int response = pthread_mutex_unlock(mutex);
      retVal = ConvertLinuxReturnToMrtsError(response);
   }

   return retVal;
}
#else
int32_t MrtsMutexUnlock( MrtsMutex_t* mutex )
{
   int32_t retVal = merNoError;

   mutex->isLocked = false;

   /*printf("MrtsMutexUnlock unlocked\n\r ");*/

   return retVal;
}
#endif

/*!\name      MrtsMutexDestroy
 *
 * \brief     Kill an existing mutex.
 *
 * \returns   mrtsError (negative fail)
 */
#ifdef LINUX_BUILD
int32_t MrtsMutexDestroy( MrtsMutex_t* mutex )
{
   int32_t retVal = merNullPointer;

   if (mutex)
   {
      int response = pthread_mutex_destroy(mutex);
      retVal = ConvertLinuxReturnToMrtsError(response);
   }

   return retVal;
}
#else
int32_t MrtsMutexDestroy( MrtsMutex_t* mutex )
{
   int32_t retVal = merNoError;

   mutex->isLocked = false;
   return retVal;
}
#endif
