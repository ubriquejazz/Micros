/**
  ******************************************************************************
  * @file    i2c.h
  * @author  
  * @version V1.0.0
  * @date    30 October 2020
  * @brief   Source file of I2C driver.
  ******************************************************************************
  */

#ifndef I2C__H__
#define I2C__H__

#define MAX_BUFFER_SIZE 20  //
#define I2C_ADDR 			0x10  //
#define SLAVE_ADDR 		0x08  // 0x10 including R/W bit or 0x8 as 7-bit address
#define CRC_Mode 			0 // 0 for disabled, 1 for enabled
#define CRC_KEY 			7 //

typedef enum {
    IDLE_MODE,
    NACK_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    SWITCH_TO_RX_MODE,
    SWITHC_TO_TX_MODE,
    TIMEOUT_MODE
} I2C_Mode;

/* For slave device with dev_addr, writes the data specified in *reg_data
 *
 * dev_addr: The slave device address. Example: SLAVE_ADDR
 *           
 * reg_addr: The register or command to send to the slave. 
 *          
 * *reg_data: The buffer to write. Example: MasterType0
 *           
 * count: The length of *reg_data. Example: TYPE_0_LENGTH
 *           
 *  */
void I2C_WriteReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t count);

/* For slave device with dev_addr, read the data specified in slaves reg_addr.
 * NOTE: the received data is available in RX_Buffer
 *
 * dev_addr: The slave device address. Example: SLAVE_ADDR
 *           
 * reg_addr: The register or command to send to the slave. 
 *           
 * count: The length of data to read. Example: TYPE_0_LENGTH
 *           
 *  */
I2C_Mode I2C_ReadReg(uint8_t dev_addr, uint8_t reg_addr, uint8_t count);

#endif