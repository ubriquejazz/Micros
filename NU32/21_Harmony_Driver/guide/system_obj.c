/* System Objects - pag 5 */

// Example: Sample Module Initialization Function 

SYS_MODULE_OBJ SAMPLE_Initialize ( const SYS_MODULE_INDEX index, const SYS_MODULE_INIT * const init )
{
	SAMPLE_MODULE_DATA 		 *pObj = (SAMPLE_MODULE_DATA *)&gObj[index]; 
	SAMPLE_MODULE_INIT_DATA *pInit = (SAMPLE_MODULE_INIT_DATA *)init;

	/* Initialize module object. */
	pObj->state 				= SAMPLE_STATE_INITALIZE;
	pObj->status 				= SYS_STATUS_BUSY;
	pObj->dataNewIsValid 		= false;
	pObj->dataProcessedIsValid	= false;
	
	if ( null != init ) {
		pObj->dataNew = pInit->dataSome; 
		pObj->dataNewIsValid = true;
	}

	return (SYS_MODULE_OBJ)pObj;
}

// Example SYS_MODULE_INIT Structure (sys_module.h)

typedef union { 

	uint8_t value;

	struct { 
		uint8_t powerState 	: 4;
		uint8_t reserved 	: 4;
	}sys;

} SYS_MODULE_INIT;

// Example: SYS_MODULE_OBJ Initialization Function Signature (sys_module.h)

/* System Module Initialization Function Pointer

	Function Pointer:

		SYS_MODULE_OBJ (* SYS_MODULE_INITIALIZE_ROUTINE) ( 

			const SYS_MODULE_INDEX index, 
			const SYS_MODULE_INIT * const init )

	Description:

		This data type is a pointer to a function that initializes a system module
		(driver, library, or system-maintained application).

	Preconditions:

		The low-level processor and board initialization must be completed before the system 
		can call the initialization functions for any modules.

	Parameters:

		index - Zero-based index of the module instance to be initialized.

		init - Pointer to the data structure containing any data necessary to initialize the module. 
			  This pointer may be null if no data is required.

	Returns:

		A handle to the instance of the module that was initialized. 
		This handle is a necessary parameter to all of the other system level routines for that module.

	Remarks:

		This function will normally only be called once during system initialization. 

*/

typedef SYS_MODULE_OBJ (* SYS_MODULE_INITIALIZE_ROUTINE) (
	const SYS_MODULE_INDEX index, 
	const SYS_MODULE_INIT * const init 
);

// Example: Sample Module Status Function 

SYS_STATUS SAMPLE_Status ( SYS_MODULE_OBJ object ) {
	SAMPLE_MODULE_DATA *pObj = (SAMPLE_MODULE_DATA *)object;
	return pObj->status; 
}

// Example: SYS_STATUS Enumeration 

/* System Module Status

	Summary:

		Identifies the current status/state of a system module

	Description:

		This enumeration identifies the current status/state of a system module (driver, library, or application).

	Remarks:

		This enumeration is the return type for the system-level status routine defined by each driver, library, 
		or application (for example, DRV_I2C_Status).

*/

typedef enum
{
	/* Indicates that a non-system defined error has occurred. The caller must call an extended status routine for the module in question to identify the error. */ 
	SYS_STATUS_ERROR_EXTENDED = -10,

	/* An unspecified error has occurred. */ 
	SYS_STATUS_ERROR = -1,

	/* The module has not yet been initialized. */ 
	SYS_STATUS_UNINITIALIZED = 0,

	/* An operation is currently in progress. */ 
	SYS_STATUS_BUSY = 1,

	/* Any previous operations have completed and the module is ready for additional operations. */ 
	SYS_STATUS_READY = 2,

	/* Indicates that the module is in a non-system defined ready/run state.
	The caller must call an extended status routine for the module in question to identify the state. */ 
	SYS_STATUS_READY_EXTENDED = 10
} SYS_STATUS;

