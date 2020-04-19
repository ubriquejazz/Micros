#ifndef _OW_CLIENT_H
#define _OW_CLIENT_H

/*!\name      OWClient.h
 *
 * \brief     Client for 1W driver
 *
 * \author    Juan Gago
 *
 */ 

#define MODULE_TYPE     '['   //!< Module type delimiter.
#define MODULE_SERIAL   '$'   //!< Module serial delimiter.
#define STOCK_CODE      '%'   //!< Module stock code delimiter.
#define WORK_ORDER      '}'   //!< Work order info delimiter.
#define MANUFACTURER    '{'   //!< Manufactured by delimiter.
#define CHIP_ID         '!'   //!< Module chip ID delimiter.
#define PROCESS         '*'   //!< Process type for the Module delimiter.
#define TERMINATOR      ']'   //!< Terminator of any field ID delimiter (This has no data attaced to it).
#define CR_VALUE        '\r'  //!< Caraige return to mark end of data.
#define LF_VALUE        '\n'  //!< Caraige return to mark end of data.

int one_init(int);
int one_pull();
 
#endif