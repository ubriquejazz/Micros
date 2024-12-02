

I2C State Machine:

The various states in the I2C Finite State Machine (FSM)are invoked through Timer interrupt. The Timer interrupt is triggered at 4 times the baud-rate which is the rate at which the I2C FSM progresses through each state. The specific state in I2C FSM that will execute depends on where the I2C bus is at during a data transaction.

**Note: To reduce interrupt latency, Shadow Register Set should be used. This is achieved by using the PRISS register and including the "SRS" attribute (iplxSRS) in Timer interrupt declaration.

Start Condition:
	The START condition is only generated if the I2C Bus is determined to be idle. The bus is idle only if both the SCL and SDA are in high. 

	I2C_SDA_LOW_START - 
		Pulls the SDA line low, SCL is still high
	I2C_SDA_LOW_START_CHECK - 
		Idle state occuring after 1/2 BRG time. SDA is low
	I2C_SCL_LOW_START - 
		After another 1/2 BRG time has expired pull SCL low. This creates START condition on the bus
	I2C_SCL_LOW_START_CHECK
		Start operation is complete. The I2C FSM then proceeds to the next stage to transmit the first byte, the slave device address byte.

Data Transfer Operation:
	Transfers each byte of data until all the data in the trasmit buffer is empty or the data in receive buffer is full. The four states pertinent to data transfer operation is mentioned below. A progression through each state denotes transfer of one byte. The states are called repeatedly to acoomplish transfer or reception of multiple byte. The data is transitions in the middle of the low level of the SCL.

	I2C_SCL_LOW_DATA_CHECK -
		This is the start of low period of clock and the SCL is set to low. 

	I2C_SCL_LOW_DATA-
		The purpose of this state is two fold. First, it outputs or receives the data bit on the SDA. Second, if all the data bytes have been transmitted or received, the FSM transitions to I2C FSM state that outputs a STOP condition on the bus.

		Each bit of data is put onto the SDA line. On the ninth clock, the SDA line is kept floating allowing the slave to ACK or NACK the byte when data is transferred from Master to Slave (WRITE). In case when data is received from Slave (READ), on the ninth clock, the Master outputs a LOW on SDA indicating more data is expected from the slave. The Master can also float the line HIGH on the ninth clock (NACK), to indicate the slave that the Master does not expect any more data.
		
		If either any of the data byte transmitted is NACKed by the slave, the rest of the data is not transmitted and the I2C FSM will prepare to initiate a STOP condition on the bus.

	I2C_SCL_HIGH_DATA - 
		This is the start of High period of the clock and SCL is pulled high.

	I2C_SCL_HIGH_DATA_CHECK -
		In a WRITE operation, the middle of the ninth clock is checked to determine if the data sent to the slave was ACKed. In case of read operation, data is sampled at the middle of the high part of SCL.

Restart Condiiton:

	I2C_SDA_HIGH_RESTART -
		SDA line is pulled high to initiate restart.

	I2C_SDA_HIGH_RESTART_CHECK 
		Keeps SDA high for 1/4 baud-rate time more from the I2C_SDA_HIGH_RESTART.

	I2C_SCL_HIGH_RESTART - 
		SCL line is pulled high after 1/4 baud-rate time more from I2C_SDA_HIGH_RESTART_CHECK.

	I2C_SCL_HIGH_RESTART_CHECK -
		SCL line is kept high for 1/4 baud-date more I2C_SCL_HIGH_RESTART

	After another 1/4 baud rate time has expired, the I2C FSM to set to the state that creates a start condition on the bus.

Stop Condition:

	I2C_SCL_SDA_LOW_STOP:    
		Set SCL and SDA low.
	I2C_SCL_SDA_LOW_STOP_CHECK:
		Keeps SCL and SDA low of 1/4 of baud-rate time.
	I2C_SCL_HIGH_STOP
		Set SCL High
	I2C_SCL_HIGH_STOP_CHECK
		Wait for another 1/4 baud rate time.
	I2C_SDA_HIGH_STOP - 
		Pull SDA high after another 1/4 baud rate time.
	I2C_SDA_HIGH_STOP_CHECK:
		Wait another 1/4 baud rate time to complete STOP condition.



 

