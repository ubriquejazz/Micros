/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
	Microchip Technology Inc.

  File Name:
	networkcoms.c

  Summary:
	This file contains the source code for the MPLAB Harmony application.

  Description:
	This file contains the source code for the MPLAB Harmony application.  It
	implements the logic of the application's state machine and it may call
	API routines of other MPLAB Harmony modules in the system, such as drivers,
	system services, and middleware.  However, it does not call any of the
	system interfaces (such as the "Initialize" and "Tasks" functions) of any of
	the modules in the system or make any assumptions about when those functions
	are called.  That is the responsibility of the configuration-specific system
	files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "networkcoms.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

NETWORKCOMS_DATA networkcomsData;
uint8_t callReq_JSONPacket[] = "{\"request\":\"call\",\"source\":0}";
uint8_t statusReq_JSONPacket[] = "{\"request\":\"status\"}";

#define ECS_UDP_BROADCAST_PORT					2027
#define ECS_TCP_PORT							3096
#define DHCP_DETECTION_TIMEOUT_PERIOD_s			30
#define TCP_CLIENT_CONNECTION_TIMEOUT_PERIOD_s 	15
#define ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s	180
#define ECS_STATUS_REQUEST_POLL_FREQUENCY_Hz	2

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
	void NETWORKCOMS_Initialize ( void )

  Remarks:
	See prototype in networkcoms.h.
 */

void NETWORKCOMS_Initialize(void)
{
	/* Place the App state machine in its initial state. */
	networkcomsData.state = NETWORKCOMS_STATE_INIT;
	networkcomsData.ecsUDPBroadcastHandle = INVALID_UDP_SOCKET;
	networkcomsData.ecsTCPSocketHandle = INVALID_SOCKET;
	networkcomsData.consolePrintOnceFlag = false;
	networkcomsData.networkLinkDropFlag = false;
}

/******************************************************************************
  Function:
	void NETWORKCOMS_Tasks ( void )

  Remarks:
	See prototype in networkcoms.h.
 */

void NETWORKCOMS_Tasks(void)
{
	//Temporary Buffer for Storage of received UDP/TCP Data
	uint8_t receivedDataBuffer[128];
	uint16_t numberDataBytesToProcess;
	
	//Link Monitor-------------------------------------------------------------------------------------------------------------------------
	//Check if the network interface has become disconnected.
	//Close any open sockets and goto NETWORKCOMS_STATE_INIT state
	if (networkcomsData.state > NETWORKCOMS_STATE_WAIT_FOR_LINK &&
		TCPIP_STACK_NetIsLinked(networkcomsData.networkInterfaceHandle) == false)
	{
		SYS_CONSOLE_MESSAGE("networkcoms[M]: Network Link is Down\r\n");

		if (networkcomsData.ecsUDPBroadcastHandle != INVALID_UDP_SOCKET)
		{
			SYS_CONSOLE_MESSAGE("networkcoms[M]: UDP Socket Closed\r\n");
			TCPIP_UDP_Close(networkcomsData.ecsUDPBroadcastHandle);
			networkcomsData.ecsUDPBroadcastHandle = INVALID_UDP_SOCKET;
		}

		if (networkcomsData.ecsTCPSocketHandle != INVALID_SOCKET)
		{
			SYS_CONSOLE_MESSAGE("networkcoms[M]: TCP Socket Closed\r\n");
			TCPIP_TCP_Close(networkcomsData.ecsTCPSocketHandle);
			networkcomsData.ecsTCPSocketHandle = INVALID_SOCKET;
		}

		networkcomsData.networkLinkDropFlag = true;
		networkcomsData.consolePrintOnceFlag = false;
		networkcomsData.state = NETWORKCOMS_STATE_WAIT_FOR_LINK;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------

	//Main State Machine-------------------------------------------------------------------------------------------------------------------
	switch (networkcomsData.state)
	{
		case NETWORKCOMS_STATE_INIT:
			//Wait for the TCPIP Stack and System Timer Service to become initialised
			if (TCPIP_STACK_Status(sysObj.tcpip) == SYS_STATUS_READY &&
				SYS_TMR_Status(sysObj.sysTmr) == SYS_STATUS_READY)
			{
				if (TCPIP_STACK_NumberOfNetworksGet() > 0)
				{
					//Get a handle to the network interface
					//For this application only one interface is used
					networkcomsData.networkInterfaceHandle = TCPIP_STACK_IndexToNet(0);

					SYS_CONSOLE_PRINT("networkcoms[1]: TCPIP Stack is Ready on %s Interface\r\n",
									  TCPIP_STACK_NetNameGet(networkcomsData.networkInterfaceHandle));
					SYS_CONSOLE_PRINT("networkcoms[1]: Host name is %s\r\n",
									  TCPIP_STACK_NetBIOSName(networkcomsData.networkInterfaceHandle));
					SYS_CONSOLE_PRINT("networkcoms[1]: DHCP Client is enabled on %s interface\r\n",
									  TCPIP_STACK_NetNameGet(networkcomsData.networkInterfaceHandle));

					networkcomsData.timeoutTick = SYS_TMR_TickCountGet();
					networkcomsData.state = NETWORKCOMS_STATE_WAIT_FOR_LINK;
				}
			}
			break;

		case NETWORKCOMS_STATE_WAIT_FOR_LINK:
			//Determine if the network interface is connected
			if (TCPIP_STACK_NetIsLinked(networkcomsData.networkInterfaceHandle))
			{
				if (networkcomsData.consolePrintOnceFlag == false)
				{
					SYS_CONSOLE_MESSAGE("networkcoms[2]: Network Link is Up\r\n");
					networkcomsData.consolePrintOnceFlag = true;
				}

				//Check if the DHCP Server has assigned an IP Address
				if (TCPIP_DHCP_IsBound(networkcomsData.networkInterfaceHandle))
				{
					networkcomsData.hostIPAddress.Val = TCPIP_STACK_NetAddress(networkcomsData.networkInterfaceHandle);
					SYS_CONSOLE_PRINT("networkcoms[2]: DHCP Client has received IP Address: %d.%d.%d.%d\r\n",
									  networkcomsData.hostIPAddress.v[0], networkcomsData.hostIPAddress.v[1], networkcomsData.hostIPAddress.v[2], networkcomsData.hostIPAddress.v[3]);
					networkcomsData.state = NETWORKCOMS_SETUP_UDP_SERVER;
				}
				else if (SYS_TMR_TickCountGet() - networkcomsData.timeoutTick >=
						 SYS_TMR_TickCounterFrequencyGet() * DHCP_DETECTION_TIMEOUT_PERIOD_s)
				{
					//If no DHCP Server is found on the network after the timeout
					//period defined by DHCP_DETECTION_TIMEOUT_PERIOD_s
					//the PIC will disable the DHCP Client, and it
					//will become a DHCP Server.

					SYS_CONSOLE_MESSAGE("networkcoms[2]: DHCP Server Detection Timeout\r\n");

					//Disable DHCP Client
					TCPIP_DHCP_Disable(networkcomsData.networkInterfaceHandle);
					SYS_CONSOLE_PRINT("networkcoms[2]: DHCP Client is disabled on %s interface\r\n",
									  TCPIP_STACK_NetNameGet(networkcomsData.networkInterfaceHandle));

					//Enable DHCP Server
					TCPIP_DHCPS_Enable(networkcomsData.networkInterfaceHandle);
					SYS_CONSOLE_PRINT("networkcoms[2]: DHCP Server is active on %s interface\r\n",
									  TCPIP_STACK_NetNameGet(networkcomsData.networkInterfaceHandle));

					networkcomsData.state = NETWORKCOMS_SETUP_UDP_SERVER;
				}
			}
			else
			{
				if (networkcomsData.networkLinkDropFlag == true)
				{
					//When the network link drops, disable the DHCP Server
					//and re-enable the DHCP Client
					networkcomsData.networkLinkDropFlag = false;

					//Since the DHCP Server is configured for only 1 entry in the pool
					//the existing entry is removed to allow a different device
					//to connect to the PIC and obtain an IP Address
					TCPIP_DHCPS_RemovePoolEntries(networkcomsData.networkInterfaceHandle, DHCP_SERVER_POOL_ENTRY_ALL);

					//Disable DHCP Server
					TCPIP_DHCPS_Disable(networkcomsData.networkInterfaceHandle);

					//Enable DHCP Client
					TCPIP_DHCP_Enable(networkcomsData.networkInterfaceHandle);

					SYS_CONSOLE_PRINT("networkcoms[2]: DHCP Client is enabled on %s interface\r\n",
									  TCPIP_STACK_NetNameGet(networkcomsData.networkInterfaceHandle));
				}
				networkcomsData.timeoutTick = SYS_TMR_TickCountGet();
			}
			break;

		case NETWORKCOMS_SETUP_UDP_SERVER:
			/* TODO:[1] Open a UDP Server Instance
			 * - IPv4 address type
			 * - Port is defined in the ECS_UDP_BROADCAST_PORT constant
			 * - Accept connections on ANY interface.
			 * - Store returned handle in the networkcomsData.ecsUDPBroadcastHandle variable*/
			networkcomsData.ecsUDPBroadcastHandle = TCPIP_UDP_ServerOpen(IP_ADDRESS_TYPE_IPV4,ECS_UDP_BROADCAST_PORT,0);//<--Insert the solution for item 1 on this line
			if (networkcomsData.ecsUDPBroadcastHandle != INVALID_UDP_SOCKET)
			{
				SYS_CONSOLE_MESSAGE("networkcoms[3]: UDP Server Opened\r\n");
				networkcomsData.state = NETWORKCOMS_LISTEN_FOR_ECS_BROADCAST;
			}
			else
			{
				SYS_CONSOLE_MESSAGE("networkcoms[3] Error Opening UDP Server\r\n");
				networkcomsData.state = NETWORKCOMS_ERROR;
			}
			break;

		case NETWORKCOMS_LISTEN_FOR_ECS_BROADCAST:
			/*TODO:[2] Check if any data has been received on the UDP Socket
             */
			if (TCPIP_UDP_GetIsReady(networkcomsData.ecsUDPBroadcastHandle) > 0) //<--Insert the solution for item 2 on this line (replace the underscores with the solution)
			{
				// Copy the data received on the UDP Socket into the receivedDataBuffer.
				// The numberDataBytesToProcess returned value will be the lesser of
				// the size of the receivedDataBuffer Array-1 or
				// the number of bytes that has been received.
				numberDataBytesToProcess = TCPIP_UDP_ArrayGet(networkcomsData.ecsUDPBroadcastHandle, receivedDataBuffer, sizeof (receivedDataBuffer - 1));

				receivedDataBuffer[numberDataBytesToProcess] = '\0';
				SYS_CONSOLE_PRINT("networkcoms[4]: Data Received on UDP Socket: %s\r\n", receivedDataBuffer);

				//Process the received data packet
				while (ECSDATAPROCESSING_Parse(&networkcomsData.ecsParseContext, receivedDataBuffer, numberDataBytesToProcess, &networkcomsData.elevatorStatusData))
				{
					//Check if the received data packet was a broadcast
					if (networkcomsData.ecsParseContext.parsedPacketType == ECS_BROADCAST)
					{
						if (TCPIP_UDP_SocketInfoGet(networkcomsData.ecsUDPBroadcastHandle,&networkcomsData.ecsUDPSocketInfo))
						{
							SYS_CONSOLE_PRINT("networkcoms[4]: ECS Broadcast found on IP %d.%d.%d.%d\r\n",
											  networkcomsData.ecsUDPSocketInfo.sourceIPaddress.v4Add.v[0],
											  networkcomsData.ecsUDPSocketInfo.sourceIPaddress.v4Add.v[1],
											  networkcomsData.ecsUDPSocketInfo.sourceIPaddress.v4Add.v[2],
											  networkcomsData.ecsUDPSocketInfo.sourceIPaddress.v4Add.v[3]);

							/*TODO:[3] Close the UDP Server Instance
                             */
							TCPIP_UDP_Close(networkcomsData.ecsUDPBroadcastHandle);//<--Insert the solution for item 3 on this line

							//Goto the Setup TCP Client State
							SYS_CONSOLE_MESSAGE("networkcoms[4]: UDP Server Closed\r\n");
							networkcomsData.ecsUDPBroadcastHandle = INVALID_UDP_SOCKET;
							networkcomsData.state = NETWORKCOMS_SETUP_TCP_CLIENT;
						}
					}
				}
			}
			break;

		case NETWORKCOMS_SETUP_TCP_CLIENT:
			/* TODO:[4] Open a TCP Client Instance
			 * - IPv4 Address Type
			 * - Destination Port is defined in ECS_TCP_PORT constant
			 * - IP Address is stored in networkcomsData.ecsUDPSocketInfo.sourceIPaddress structure
			 * - Store returned handle in the networkcomsData.ecsTCPSocketHandle variable.
			 * Hint: You must pass the address of networkcomsData.ecsUDPSocketInfo.sourceIPaddress 
			 * to the Harmony API that is used to open the TCP Client.*/
			networkcomsData.ecsTCPSocketHandle = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4,ECS_TCP_PORT,&networkcomsData.ecsUDPSocketInfo.sourceIPaddress);//<--Insert the solution for item 4 on this line
			if (networkcomsData.ecsTCPSocketHandle != INVALID_SOCKET)
			{
				SYS_CONSOLE_MESSAGE("networkcoms[5]: TCP Client Opened\r\n");
				networkcomsData.timeoutTick = SYS_TMR_TickCountGet();
				networkcomsData.state = NETWORKCOMS_WAIT_FOR_TCP_CONNECTION;
			}
			else
			{
				SYS_CONSOLE_MESSAGE("networkcoms[5]: Error Opening TCP Client\r\n");
				networkcomsData.state = NETWORKCOMS_ERROR;
			}
			break;

		case NETWORKCOMS_WAIT_FOR_TCP_CONNECTION:
			/*TODO:[5] Check if the TCP Client is connected to the Server 
             */
			if (TCPIP_TCP_IsConnected(networkcomsData.ecsTCPSocketHandle)) //<--Insert the solution for item 5 on this line (replace the underscores with the solution)
			{
				SYS_CONSOLE_MESSAGE("networkcoms[6]: TCP Socket Connected\r\n");

				//Clear the TCP Connection Reset Semaphore
				TCPIP_TCP_WasReset(networkcomsData.ecsTCPSocketHandle);

				//Load start tick to allow the next 500ms period to be timed
				networkcomsData.statusRequestTick = SYS_TMR_TickCountGet();
				networkcomsData.consolePrintOnceFlag = false;
				networkcomsData.callRequestState = CALLREQ_CHECK_FOR_NEW_REQUEST;
				networkcomsData.statusRequestState = STATUSREQ_WAIT_FOR_TIMER;
				networkcomsData.state = NETWORKCOMS_MANAGE_TCP_CLIENT;
			}
			else
			{
				//If the TCP Connection is not established after a period
				//defined by TCP_CLIENT_CONNECTION_TIMEOUT_PERIOD_s, the socket
				//will be closed.
				if (SYS_TMR_TickCountGet() - networkcomsData.timeoutTick >=
					SYS_TMR_TickCounterFrequencyGet() * TCP_CLIENT_CONNECTION_TIMEOUT_PERIOD_s)
				{
					SYS_CONSOLE_MESSAGE("networkcoms[6]: TCP Socket could not be established\r\n");
					networkcomsData.state = NETWORKCOMS_CLOSE_TCP_CLIENT;
				}
			}
			break;

		case NETWORKCOMS_MANAGE_TCP_CLIENT:
			/* TODO:[6] Check if the TCP connection has dropped out 
			 * any time between the current and previous execution 
			 * of the NETWORKCOMS_MANAGE_TCP_CLIENT state.*/
			if (TCPIP_TCP_WasReset(networkcomsData.ecsTCPSocketHandle)) //<--Insert the solution to item 6 on this line (replace the underscores)
			{
				SYS_CONSOLE_MESSAGE("networkcoms[7]: TCP Socket Disconnected\r\n");
				//If the connection has dropped out, close the TCP Socket
				networkcomsData.state = NETWORKCOMS_CLOSE_TCP_CLIENT;
			}
			else
			{
				//Data Reception Management------------------------------------------------------------------------------------------------
				//Check if any data has been received on the socket
				if (TCPIP_TCP_GetIsReady(networkcomsData.ecsTCPSocketHandle) > 0)
				{
					//Copy the received data into the receivedDataBuffer.
					numberDataBytesToProcess = TCPIP_TCP_ArrayGet(networkcomsData.ecsTCPSocketHandle, receivedDataBuffer, sizeof (receivedDataBuffer));

					//ECS_DATAPROCESSING_Parse will return with TRUE when there is a complete parsed data packet available.
					//There still may be unprocessed data in the receivedDataBuffer, hence the Parse function is
					//continuously called until there is no more complete data packets.
					while (ECSDATAPROCESSING_Parse(&networkcomsData.ecsParseContext, receivedDataBuffer,
												   numberDataBytesToProcess, &networkcomsData.elevatorStatusData))
					{
						if (networkcomsData.ecsParseContext.parsedPacketType == ECS_RESPONSE_CALLACK &&
							networkcomsData.callRequestState == CALLREQ_WAIT_FOR_RESPONSE)
						{
							networkcomsData.callRequestState = CALLREQ_CHECK_FOR_NEW_REQUEST;
							SYS_CONSOLE_MESSAGE("networkcoms[7]: Received Call Acknowledgement from ECS \r\n");
						}
						else if (networkcomsData.ecsParseContext.parsedPacketType == ECS_RESPONSE_STATUS &&
								 networkcomsData.statusRequestState == STATUSREQ_WAIT_FOR_RESPONSE)
						{
							networkcomsData.statusRequestState = STATUSREQ_REARM_TIMER;
						}
					}
				}
				//-------------------------------------------------------------------------------------------------------------------------

				//Management of the Call Request-------------------------------------------------------------------------------------------
				switch (networkcomsData.callRequestState)
				{
					case CALLREQ_CHECK_FOR_NEW_REQUEST:
						//Check if any of the call buttons have been pressed
						if (BUTTONCONTROL_GetCallButtonState() > ECS_CALL_NONE)
						{
							networkcomsData.callRequestState = CALLREQ_TRANSMIT_REQUEST;
							SYS_CONSOLE_PRINT("networkcoms[7]: Call Button %d Pressed\r\n",
											  (uint8_t) BUTTONCONTROL_GetCallButtonState());
						}
						break;

					case CALLREQ_TRANSMIT_REQUEST:
						/* TODO:[7] Get the amount of free space
						 * available in the Transmit Buffer of the TCP Socket */
						if (TCPIP_TCP_PutIsReady(networkcomsData.ecsTCPSocketHandle) >= strlen(callReq_JSONPacket)) //<--Insert the solution to item 7 on this line (replace the underscores with the solution)
						{
							//Modify the "call source" data value in the JSON packet
							callReq_JSONPacket[27] = (uint8_t) BUTTONCONTROL_GetCallButtonState() + '0';

							//Clear the call request latches to prevent multiple
							//call requests from being sent while the button is pressed
							BUTTONCONTROL_ClearCallButtonLatches();

							//Load a new tick which is used for a timeout on the reception of the response
							networkcomsData.timeoutTick = SYS_TMR_TickCountGet();

							/*TODO:[8] Send the string stored in callReq_JSONPacket array to the TCP Server
                             */
							TCPIP_TCP_ArrayPut(networkcomsData.ecsTCPSocketHandle, callReq_JSONPacket, strlen(callReq_JSONPacket));//<--Insert the solution to item 8 on this line
							SYS_CONSOLE_MESSAGE("networkcoms[7]: Call Request Transmitted to ECS\r\n");

							networkcomsData.callRequestState = CALLREQ_WAIT_FOR_RESPONSE;
						}
						break;

					case CALLREQ_WAIT_FOR_RESPONSE:
						//When an acknowledgment is received the callRequestState will
						//change to CALLREQ_CHECK_FOR_NEW_REQUEST (this is performed by the data reception procssing code)

						//Check if a response has been received within a time window
						//defined by ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s
						if (SYS_TMR_TickCountGet() - networkcomsData.timeoutTick >=
							SYS_TMR_TickCounterFrequencyGet() * ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s)
						{
							SYS_CONSOLE_MESSAGE("networkcoms[7]: Call Request: Response Timeout\r\n");
							//If no response has been received, close the TCP Socket
							networkcomsData.state = NETWORKCOMS_CLOSE_TCP_CLIENT;
						}
						break;

					default:
						networkcomsData.callRequestState = CALLREQ_CHECK_FOR_NEW_REQUEST;
						break;
				}
				//-------------------------------------------------------------------------------------------------------------------------

				//Management of the Status Request-----------------------------------------------------------------------------------------
				switch (networkcomsData.statusRequestState)
				{
					case STATUSREQ_WAIT_FOR_TIMER:
						//Check if 500ms has expired
						if (SYS_TMR_TickCountGet() - networkcomsData.statusRequestTick >=
							SYS_TMR_TickCounterFrequencyGet() / ECS_STATUS_REQUEST_POLL_FREQUENCY_Hz)
						{
							networkcomsData.statusRequestState = STATUSREQ_TRANSMIT_REQUEST;
						}
						break;

					case STATUSREQ_TRANSMIT_REQUEST:
						if (TCPIP_TCP_PutIsReady(networkcomsData.ecsTCPSocketHandle) >= strlen(statusReq_JSONPacket))
						{
							TCPIP_TCP_StringPut(networkcomsData.ecsTCPSocketHandle, statusReq_JSONPacket);

							//Load a new tick which is used for a timeout on the reception of the response
							networkcomsData.statusRequestTick = SYS_TMR_TickCountGet();

							networkcomsData.statusRequestState = STATUSREQ_WAIT_FOR_RESPONSE;
						}
						break;

					case STATUSREQ_WAIT_FOR_RESPONSE:
						//Once the status response is received, the statusRequestState will
						//change to STAT_REQ_REARM_TIMER (this is performed by the data reception procssing code)

						//Check if a response has been received within a time window
						//defined by ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s
						if (SYS_TMR_TickCountGet() - networkcomsData.statusRequestTick >=
							SYS_TMR_TickCounterFrequencyGet() * ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s)
						{
							SYS_CONSOLE_MESSAGE("networkcoms[7]: Status Request: Response Timeout\r\n");

							//If no response has been received, close the TCP Socket
							networkcomsData.state = NETWORKCOMS_CLOSE_TCP_CLIENT;
						}
						break;

					case STATUSREQ_REARM_TIMER:
						//Reload start tick to allow the next 500ms period to be timed
						networkcomsData.statusRequestTick = SYS_TMR_TickCountGet();

						//Update the call indicator lamps
						LEDCONTROL_SetCallIndicators(networkcomsData.elevatorStatusData.callindicators);

						if (networkcomsData.consolePrintOnceFlag == false)
						{
							SYS_CONSOLE_PRINT("networkcoms[7]: ECS Assignment: Elevator: %d, Floor: %d\r\n",
											  networkcomsData.elevatorStatusData.carID, networkcomsData.elevatorStatusData.floorID);

							networkcomsData.consolePrintOnceFlag = true;
						}

						networkcomsData.statusRequestState = STATUSREQ_WAIT_FOR_TIMER;
						break;

					default:
						networkcomsData.statusRequestState = STATUSREQ_REARM_TIMER;
						break;
				}
				//-------------------------------------------------------------------------------------------------------------------------
			}
			break;

		case NETWORKCOMS_CLOSE_TCP_CLIENT:
			TCPIP_TCP_Close(networkcomsData.ecsTCPSocketHandle);
			networkcomsData.ecsTCPSocketHandle = INVALID_SOCKET;

			SYS_CONSOLE_MESSAGE("networkcoms[8]: TCP Client Closed\r\n");
			networkcomsData.state = NETWORKCOMS_SETUP_UDP_SERVER;
			break;

		case NETWORKCOMS_ERROR:

			break;

		default:
			break;
	}
}

/*******************************************************************************
 End of File
 */