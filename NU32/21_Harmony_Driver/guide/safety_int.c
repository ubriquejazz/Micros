/* Safety Interrupts - pag 23 */

// Example: Interrupt Management 

	bool SAMPLE_DataGet ( const SYS_MODULE_INDEX index, int *data ) 
	{
		SAMPLE_MODULE_DATA *pObj; 
		bool intState; 
		bool result = false; 
		pObj = (SAMPLE_MODULE_DATA *)&gObj[index]; 

		// Guard against interrupts 
		intState = _SAMPLE_InterruptDisable(pObj->interrupt); 
		if (pObj->dataProcessedIsValid) {
			// Provide data
			*data = pObj->dataProcessed;
			pObj->dataProcessedIsValid = false;
			result = true; 
		}
		
		/* Restore interrupt state and unlock module object. */ 
		_SAMPLE_InterruptRestore(pObj->interrupt, intState); 
		return result;
	}


// Example: Temporarily Disabling an Interrupt Source 

	#define MY_INTERRUPT_SOURCE INT_SOURCE_TIMER_2

	bool enabled;
	enabled = SYS_INT_SourceDisable(MY_INTERRUPT_SOURCE); 

	/* Access resource shared with interrupt-driven tasks function. */
	if (enabled) { 
		SYS_INT_SourceEnable(MY_INTERRUPT_SOURCE); 
	}	

// Example: Interrupt Management Functions 

	#if (SAMPLE_MODULE_INTERRUPT_MODE == true)
		#define _SAMPLE_InterruptDisable(s) SYS_INT_SourceDisable(s)

		static inline void _SAMPLE_InterruptRestore ( INT_SOURCE source, bool enabled ) { 
			if (enabled) { 
				SYS_INT_SourceEnable(source);
			}
		} 
	#else
		#define _SAMPLE_InterruptDisable(s) 	false
		#define _SAMPLE_InterruptRestore(s,e)	false
	#endif



