#ifndef _CLIENTS__H__
#define _CLIENTS__H__

/*!\name      clients.c
 *
 * \brief     Local clients: MCP9808_{},  RFE1600_{}, etc.
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

/* Conversion */
int8_t MCP9808_Temp  (uint16_t y); // uint8_t upper, uint8_t lower
float  RFE1600_Vout  (uint16_t y);
float  RFE1600_Iout  (uint16_t y);
float  RFE1600_Temp  (uint16_t y);

/* I2C Transaction */
bool MCP9808_Rd_Word  (I2C_CLIENT*, MCP9808_ADDR, MCP9808_REG);

bool RFE1600_Rd_Byte  (I2C_CLIENT*, RFE1600_ADDR, RFE1600_REG);
bool RFE1600_Rd_Word  (I2C_CLIENT*, RFE1600_ADDR, RFE1600_REG);
bool RFE1600_Wr_Block (I2C_CLIENT*, RFE1600_ADDR, RFE1600_REG, const uint8_t*, uint8_t);

#endif