
#ifndef _CONSOLE_H    /* Guard against multiple inclusion */
#define _CONSOLE_H

/*!\brief     InfraRed Messages */

#define CONSOLE_IR_BEACON_ACK   'A'
#define CONSOLE_IR_BEACON_REQ   'B'
#define CONSOLE_IR_MESSAGE_REQ  'M'
#define CONSOLE_IR_REPLY_START  "* "
#define CONSOLE_U0_REPLY_START  "\n* "
#define CONSOLE_IR_REPLY_END    "\r"
#define CONSOLE_IR_ACK          "* ACK F321\r"
#define CONSOLE_IR_NACK         "* NACK 1FFD\r"

#if(PROD0)
#define CONSOLE_IR_REQ_BT       "! BT%d"            
#define CONSOLE_IR_REQ_BC       "! BC%d %04X"
#define CONSOLE_IR_REQ_MS       "! MS%d %04X %s"
#define CONSOLE_IR_REQ_MR       "! MR%d %04X"
#else
#define CONSOLE_IR_REQ_BT       "$BT%d"             // argc = 2
#define CONSOLE_IR_REQ_BC       "$BC%d %04X"        // argc = 3
#define CONSOLE_IR_REQ_MS       "$MS%d %04X %s"     // argc = 4
#define CONSOLE_IR_REQ_MR       "$MR%d %04X"        // argc = 3
#endif

#if(PROD1)
#define CONSOLE_EXAMPLE_COMMAND "! 123 SMM MST BLADE_CTRL READ_MASTER_VER C08B"
#define CONSOLE_EXAMPLE_ANSWER  "* 123 MST SMM OK ABCD 71A6" 
#else
#define CONSOLE_EXAMPLE_COMMAND "PAYLOAD-REQ"
#define CONSOLE_EXAMPLE_ANSWER  "PAYLOAD-ACK"
#endif

// Ack format: * Axy ID STATUS 
#define CONSOLE_IR_ACK_BT       "* AT%d %04X %1X"       // argc = 5
#define CONSOLE_IR_ACK_BC       "* AC%d %04X %1X"       // argc = 5
#define CONSOLE_IR_ACK_MS       "* AS%d %04X %1X"       // argc = 5
#define CONSOLE_IR_ACK_MR       "* AR%d %04X %1X %s"    // argc = 6 (at least)

/*!\brief     Console Messages. */

#define CONSOLE_ERR_SIMPLE_MESSAGE      "\n__ERR %s\r"
#define CONSOLE_ERR_CMD_NOT_RECOGNIZED  "\n__ERR No recognized \"%s\"\r"
#define CONSOLE_ERR_FAILING_CRC_CHECK   "\n__ERR crc_rx=%4x expected=%4x\r"
#define CONSOLE_ERR_INVALID_CRC_FORMAT  "\n__ERR Invalid CRC format %s\r"
#define CONSOLE_ERR_ACK_ID_FFFF         "\n__ERR %d ID=%04X" 
#define CONSOLE_ERR_ACK_INVALID_ID      "\n__ERR %d ID=%04X (%04X)"
#define CONSOLE_ERR_ACK_INVALID_STATUS  "\n__ERR %d STATUS=%02X"

#define CONSOLE_ERR_INT_I2CLOOP         "__ERR I2C in an internal loop\r\n"
#define CONSOLE_ERR_I2C_SENSOR          "__ERR Only sensor 31 and 38 work\r\n"
#define CONSOLE_INFO_CMD_IGNORED        "__INFO Last command ignored\r\n"
#define CONSOLE_INFO_WR_RD_EXEC         "__INFO WrRd operation executed\r\n"


#endif