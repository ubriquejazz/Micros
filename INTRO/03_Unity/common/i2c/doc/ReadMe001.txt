

void I2C_Initialize ( uint32_t baudrate, bool masterISREnable, bool busISREnable )

	Summary:
		This functions transfers data from the Master to the Slave. 

	Function Parameters:
		baudrate - sets the baud rate at which data transfer takes place. This value is used to calculate the timer value.
		
		masterISR.Enable  - decides if the I2C Master interrupt using the IECx and IFSx registers is enabled in software. 
							This interrupt will be triggered by software upon completion of a STOP condition
		
		busISR.Enable - decides if the I2C Bus interrupt using IECx and IFSx registers is enabled in software. 
						This will be triggered by software when a bus collision is detected.

	Description:
		The first step is to initialize the I2C peripheral. The SCL and SDA pins are configured as inputs with their respective Open-Dran configuration enabled. The baud-rate is accepted as a function parameter and is used to calculate the rate at which Timer interrupt happens. The timer period is set to trigger at 4 times the baud-rate which enables data transition in the middle of clock and to read the middle of high side of clock when the data is stable according to I2C specifications.  The Timer Interrupt is responsible for providing correct timing for START, STOP, RESTART and byte transfer operations.

		*Note: I2C Master interrupt and Bus Exception interrupt refer to the same CPU interrupt that is defined in the Interrupt Vector Table and must be enabled and triggered using corresponding bits in IECx and IFSx registers respectively.


I2C_OPERATION_STATUS DRV_I2C_MasterBufferWrite (uint16_t deviceaddress, uint8_t* txBuffer, uint16_t txbuflen)

	Summary:
		This functions transfers data from the Master to the Slave. 

	Function Parameters:
		deviceaddress 	- device address of the slave
		*txBuffer 		- pointer to data buffer that contains data to be written to the slave
		txbuflen		- number of bytes to be transferred to the slave

	Returns:
		Status of I2C Bus

	Description:
		When the function is called, the status of the I2C bus is checked using i2cStatus before a buffer transfer operation commences on the I2C bus. If the flag i2cStatus is set at I2C_STOP_COMPLETE,I2C state machine is kicked off by invoking StartI2C(). For any other value of i2cStatus, it implies that an I2C transaction is in progress and the the function call returns an error.


I2C_OPERATION_STATUS DRV_I2C_MasterBufferRead (uint16_t deviceaddress, uint8_t* rxBuffer, uint16_t rxbuflen)

	Summary:
	This function enables Master to read data from a slave device. 

	Functiona Parameters:
		deviceaddress 	- device address of the slave, 
		*rxBuffer	- pointer to the data buffer into which data from slave is to be read
				  	since this is a read operation, the R/W bit in the slave address will be set before sending out the address
		rxbuflen	- Number of bytes to be read from the slave

	Returns:
		Status of I2C Bus

	Description:
		When the function is called, the status of the I2C bus is checked using the flag, i2cStatus before a buffer transfer operation commences on the I2C bus. If the flag i2cStatus is set at I2C_STOP_COMPLETE,I2C state machine is kicked off by invoking StartI2C(). For any other value of i2cStatus, it implies that an I2C transaction is in progress and the the function call returns an error.
		This function reads the slave device, the number of data bytes that is read is specified by by the number of bytes to be read from slave (rxbuflen)


I2C_OPERATION_STATUS DRV_I2C_MasterBufferWriteRead (uint16_t deviceaddress, 
uint8_t* txBuffer, uint16_t txbuflen, uint8_t* rxBuffer, uint16_t rxbuflen)

	Summary:
		This function enables Master to write and read data from a slave device without relinquishing the I2C bus. 

	Function Parameters:
		deviceaddress 	- device address of the slave
		*txBuffer 	- pointer to data buffer that contains data to be written to the slave
		txbuflen	- number of bytes to be transferred to the slave
		*rxBuffer	- pointer to the data buffer into which data from slave is to be read
				  since this is a read operation, the R/W bit in the slave address will be set before sending out the address
		rxbuflen	- Number of bytes to be read from the slave

	Returns:
		Status of I2C Bus

	Description:
		When the function is called, the status of the I2C bus is checked using the flag, i2cStatus before a buffer transfer operation commences on the I2C bus. If the flag i2cStatus is set at I2C_STOP_COMPLETE,I2C state machine is kicked off by invoking StartI2C(). For any other value of i2cStatus, it implies that an I2C transaction is in progress and the the function call returns an error.
		This function is used to index to a slave register address and read the contents thereon as specified by the number of bytes to be read from slave (rxbuflen)


I2C_OPERATION_STATUS DRV_I2C_MASTEROpStatus (void)

	Summary: 
		This function can be called to check the status of an I2C operation.

	Returns:
		Status of I2C operation

	Description:
		This function returns the status of an I2C operation. If the status returned is other than I2C_STOP_COMPLETE, a new I2C transaction cannot be started on the bus.
