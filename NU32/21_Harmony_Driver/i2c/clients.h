#ifndef _CLIENTS__H__
#define _CLIENTS__H__

/*!\name      clients.c
 *
 * \brief     I2C Clients, MCP9808_{},  RFE1600_{}
 *
 * \author    Juan Gago
 *
 */

typedef enum {
} MCP9808_REG;

typedef enum {
} MCP9808_ADDR;

typedef enum {
} RFE1600_REG;

typedef enum {
} RFE1600_ADDR;

/* Sensor */
int8_t MCP9808_Degrees (uint8_t upper, uint8_t lower);
bool MCP9808_Write (I2C_CLIENT*, MCP9808_ADDR, MCP9808_REG, uint16_t);
bool MCP9808_Read  (I2C_CLIENT*, MCP9808_ADDR);

/* PSU - Write */
bool  RFE1600_Write (I2C_CLIENT*, RFE1600_REG, uint16_t);
bool  RFE1600_Read  (I2C_CLIENT*, RFE1600_REG, uint8_t);
float RFE1600_Vout  (uint16_t y);
float RFE1600_Iout  (uint16_t y);
float RFE1600_Temp  (uint16_t y);
bool  RFE1600_Command_Vout (I2C_CLIENT* ptr, float x);

#endif