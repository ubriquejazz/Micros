/* Safety RTOS - pag 24 */

// Example: Guarding Shared Resources

	bool SAMPLE_DataGet (...)
	{
		bool result = false; 

		/* Guard against other clients */
		if (OSAL_MUTEX_Lock(&pObj->mutex, SAMPLE_MODULE_TIMEOUT) == OSAL_RESULT_TRUE) 
		{
			/* Guard against interrupts */ 
			intState = _SAMPLE_InterruptDisable(pObj->interrupt);

			/* Check for storage space */ 
			if (!pObj->dataNewIsValid) { 
				/* Store data */ 
				pObj->dataNew = data; 
				pObj->dataNewIsValid = true; 
				pObj->status = SYS_STATUS_BUSY; 
				result = true; 
			}

			/* Restore interrupt state and unlock module object. */ 
			_SAMPLE_InterruptRestore(pObj->interrupt, intState); 
			OSAL_MUTEX_Unlock(&pObj->mutex);
		}
		return result;
	}

// Example: OSAL Use in Interrupt Tasks Functions 

	#if (SAMPLE_MODULE_INTERRUPT_MODE == false)

		static inline bool _SAMPLE_TasksMutexLock (SAMPLE_MODULE_DATA *pObj) 
		{ 
			if (OSAL_MUTEX_Lock(&pObj->mutex, SAMPLE_MODULE_TIMEOUT) == OSAL_RESULT_TRUE) { 
				return true; 
			} 
			return false; 
		} 
		static inline void _SAMPLE_TasksMutexUnlock ( SAMPLE_MODULE_DATA *pObj ) 
		{ 
			OSAL_MUTEX_Unlock(&pObj->mutex); 
		} 

	#else 
		#define _SAMPLE_TasksMutexLock(p) 	true
		#define _SAMPLE_TasksMutexUnlock(p) true
	#endif

// Example: Guarding Shared Resources in Interrupt-Driven Tasks 

	void SAMPLE_Tasks( SYS_MODULE_OBJ object ) 
	{
		SAMPLE_MODULE_DATA *pObj = (SAMPLE_MODULE_DATA *)object; 
		SYS_ASSERT(_ObjectIsValid(object), "Invalid object handle"); 

		/* Guard against other clients */
		if (!_SAMPLE_TasksMutexLock(pObj)) {
			return; 
		} 

		// Process data when ready.
		if (pObj->dataNewIsValid && !pObj->dataProcessedIsValid) {

			pObj->dataProcessed = pObj->dataNew;
			pObj->dataNewIsValid = false;
			pObj->dataProcessedIsValid = true;
			pObj->status = SYS_STATUS_READY; 
		}
		/* Restore and unlock module object. */
		_SAMPLE_TasksMutexUnlock(pObj); 
		return;
	}


