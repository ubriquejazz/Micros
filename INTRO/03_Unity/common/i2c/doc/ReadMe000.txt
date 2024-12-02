
I2C Master ISR State Machine:

	The I2C Master Finite State Machine (FSM) provides the user with the ability to transfer or receive a buffer of bytes over the I2C protocol. 

	The I2C Master FSM uses Timer Interrupt to provide appropriate timing to generate START, STOP, RESTART and byte read or write. In case of byte read or write, Timer interrupt is responsible for generating clocks on SCL. The Timer interrupt is running at 4 times the Baud-rate. Since interrupt mechanism is used, it is the responsibility of the user to ensure that global interrupts are turned ON.

Variable Definitions:

	I2C.SWCounter - uint16_t
	Holds the count of clocks during a data transfer. The initial value of this variable is set to 9 and is decremented after SCL clock during data transfer. The value of 9, accounts for 8 data bits and 1 ACK bit.
	  
	I2C.SWData - uint16_t
	Holds the data to be transferred. The data is shifted on each clock pulse and the most significant bit of the data is output on SDA at each clock.

	I2C.NACKOut - bool
	Determines if the byte being read should be ACKed or NACKed.

	I2C.ACKStatus - bool
	Checking the value of the flag will indicate if the byte was ACKed by the slave. A value of 1, indicates the byte was ACKed while a 0 indicates the byte was NACKed.

	I2C.ReadData - uint16_t
	Holds the value that is being received from the slave.

	uint16_t i2cBaudTime - 
	holds the 4 times the baud-rate value. This value is used by the timer to set its period.

	uint16_t i2cDeviceaddress - 
	Holds the slave device address.

	uint8_t* i2cDevicetxBuffer -
	Holds the pointer to the transmit data buffer.

	uint8_t i2cDevicetxlen -
	Determines the number of bytes of data that is to be transmitted to the slave. The number of bytes does not include the slave device address.

	uint8_t* i2cDevicerxBuffer - 
	Holds the pointer to the data received from the slave.

	uint16_t i2cDevicerxlen -
	Determines the number of data bytes to be received from the slave device. 


