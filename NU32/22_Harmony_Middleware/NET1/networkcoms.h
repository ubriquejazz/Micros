#ifndef _NETWORKCOMS_H
#define _NETWORKCOMS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "ecsdataprocessing.h"

/*!\name      network-coms.c
 *
 * \brief     Network communications
 *
 * \author    Juan Gago
 *
 */

typedef enum
{
    NETWORKCOMS_STATE_INIT = 0,
    NETWORKCOMS_STATE_WAIT_FOR_LINK,
    NETWORKCOMS_SETUP_UDP_SERVER,
    NETWORKCOMS_LISTEN_FOR_ECS_BROADCAST,
    NETWORKCOMS_SETUP_TCP_CLIENT,
    NETWORKCOMS_WAIT_FOR_TCP_CONNECTION,
    NETWORKCOMS_MANAGE_TCP_CLIENT,
    NETWORKCOMS_CLOSE_TCP_CLIENT,
    NETWORKCOMS_ERROR,
} NETWORKCOMS_STATES;

typedef enum
{
    CALLREQ_CHECK_FOR_NEW_REQUEST = 0,
    CALLREQ_TRANSMIT_REQUEST,
    CALLREQ_WAIT_FOR_RESPONSE,
} NETWORKCOMS_CALLREQ_STATES;

typedef enum
{
    STATUSREQ_WAIT_FOR_TIMER = 0,
    STATUSREQ_TRANSMIT_REQUEST,
    STATUSREQ_WAIT_FOR_RESPONSE,
    STATUSREQ_REARM_TIMER,
} NETWORKCOMS_STATUSREQ_STATES;

typedef struct
{
	//The application's current state
	NETWORKCOMS_STATES state;

	//State variable for transmitting a call request
	NETWORKCOMS_CALLREQ_STATES callRequestState;
    
	//State variable for transmitting and receiving a status request
	NETWORKCOMS_STATUSREQ_STATES statusRequestState;
			
	//Handle for the network interface
	TCPIP_NET_HANDLE networkInterfaceHandle;
	
	//IP Address assigned to DHCP Client
	IPV4_ADDR hostIPAddress;
	
	//Handle for UDP Server
	UDP_SOCKET ecsUDPBroadcastHandle;
	
	//Structure to hold information about the UDP Socket
	UDP_SOCKET_INFO ecsUDPSocketInfo;
	
	//Handle for TCP Client
	TCP_SOCKET ecsTCPSocketHandle;
	
	//System Timer Tick Values
	uint32_t statusRequestTick;
	uint32_t timeoutTick;
	
	//Context storage for the ECS Data Processing Parse Function
	ECS_PARSER_CONTEXT_TYPE ecsParseContext;

	//Storage of Elevator Status Data
	ELEVATOR_STATUS_TYPE elevatorStatusData;
	
	//Flag to allow only one printout of a message to the console.
	bool consolePrintOnceFlag;
	
	//Flag to signify when the network link has dropped
	bool networkLinkDropFlag;	

} NETWORKCOMS_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

void NETWORKCOMS_Initialize(void);

void NETWORKCOMS_Tasks(void);


#endif /* _NETWORKCOMS_H */

/*******************************************************************************
 End of File
 */

