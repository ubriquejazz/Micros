
#ifndef TICOMM_H_
#define TICOMM_H_

/**
  * @brief  Compute the Checksum used in "Data R-W Access"
  */
uint8_t TICOMM_Checksum(uint8_t *ptr, uint8_t len);

/**
  * @brief  Write "Data R-W Access"
  * @param  dev_addr	slave device
  * @param  addr  		16 bit address (subcommand)
  * @param  data[] 		
  * @param  count   	length of data
  * @param  crc 		{0,1}
**/
idn_RetVal_t TIComm_WriteFlash (uint8_t dev_addr, uint8_t crc,
								uint8_t write[], uint8_t wlen)
{
	static uint8_t crc_local[2];
	if (__i2c_master_request() == IDN_OK)
	{
		// 0x3E Write addr
		IDN_HAL_I2C_Mem_Write_IT(dev_addr, 0x3E, write, wlen);

		// 0x60 Write checksum
		crc_local[0] = TICOMM_Checksum(write, wlen);
		crc_local[1] = 4+count;  // Length
		IDN_HAL_I2C_Mem_Write_IT(dev_addr, 0x60, crc_local, 2);    
	}
	__i2c_master_release()
}

/**
  * @brief  Read "Data R-W Access"
  * @param  dev_addr	slave device
  * @param  addr  		16 bit address (subcommand)
  * @param  data[] 		
  * @param  count   	length of data
  * @param  crc 		{0,1}
**/
idn_RetVal_t TIComm_ReadFlash ( uint8_t dev_addr, uint8_t crc,
								uint8_t write[], uint8_t wlen, 
								uint8_t read[], uint8_t rlen) 
{
	if (__i2c_master_request() == IDN_OK)
	{
		// 0x3E Write addr
		IDN_HAL_I2C_Mem_Write_IT(dev_addr, 0x3E, write, wlen);

		// 0x40 Read data
		IDN_HAL_I2C_Mem_Read_IT (dev_addr, 0x40, read, rlen);
	}
	__i2c_master_release()
}

// Direct -------------------------------------------------------------

/**
  * @brief  Write Register.
  */
retVal  TICOMM_WriteReg(ADDR, REG, *DATA, lenght, CRC);
{
	if (__i2c_master_request() == IDN_OK)
	{
		Mem_Write()
	}
}

/**
  * @brief  Read Register.
  */
retVal  TICOMM_ReadReg(ADDR, REG, *DATA, lenght, CRC);
{
	if (__i2c_master_request() == IDN_OK)
	{
		Mem_Read()
	}
}

#endif /* TICOMM_H_ */