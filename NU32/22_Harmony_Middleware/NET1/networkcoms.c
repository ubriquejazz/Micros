#include "networkcoms.h"

// Global Data Definitions

NETWORKCOMS_DATA netwData;

uint8_t callReq_JSONPacket[] = "{\"request\":\"call\",\"source\":0}";
uint8_t statusReq_JSONPacket[] = "{\"request\":\"status\"}";

#define ECS_UDP_BROADCAST_PORT					2027
#define ECS_TCP_PORT							3096
#define DHCP_DETECTION_TIMEOUT_PERIOD_s			30
#define TCP_CLIENT_CONNECTION_TIMEOUT_PERIOD_s 	15
#define ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s	180
#define ECS_STATUS_REQUEST_POLL_FREQUENCY_Hz	2

/*******************************************************************************
  Function:
	void NETWORKCOMS_Initialize ( void )

  Remarks:
	See prototype in networkcoms.h.
 */

void NETWORKCOMS_Initialize(void)
{
	/* Place the App state machine in its initial state. */
	netwData.state = NETWORKCOMS_STATE_INIT;
	netwData.ecsUDPBroadcastHandle = INVALID_UDP_SOCKET;
	netwData.ecsTCPSocketHandle = INVALID_SOCKET;
	netwData.consolePrintOnceFlag = false;
	netwData.networkLinkDropFlag = false;
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
	if (netwData.state > NETWORKCOMS_STATE_WAIT_FOR_LINK &&
		TCPIP_STACK_NetIsLinked(netwData.networkInterfaceHandle) == false)
	{
		SYS_CONSOLE_MESSAGE("networkcoms[M]: Network Link is Down\r\n");

		if (netwData.ecsUDPBroadcastHandle != INVALID_UDP_SOCKET)
		{
			SYS_CONSOLE_MESSAGE("networkcoms[M]: UDP Socket Closed\r\n");
			TCPIP_UDP_Close(netwData.ecsUDPBroadcastHandle);
			netwData.ecsUDPBroadcastHandle = INVALID_UDP_SOCKET;
		}

		if (netwData.ecsTCPSocketHandle != INVALID_SOCKET)
		{
			SYS_CONSOLE_MESSAGE("networkcoms[M]: TCP Socket Closed\r\n");
			TCPIP_TCP_Close(netwData.ecsTCPSocketHandle);
			netwData.ecsTCPSocketHandle = INVALID_SOCKET;
		}

		netwData.networkLinkDropFlag = true;
		netwData.consolePrintOnceFlag = false;
		netwData.state = NETWORKCOMS_STATE_WAIT_FOR_LINK;
	}
	//-------------------------------------------------------------------------------------------------------------------------------------

	//Main State Machine-------------------------------------------------------------------------------------------------------------------
	switch (netwData.state)
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
					netwData.networkInterfaceHandle = TCPIP_STACK_IndexToNet(0);

					SYS_CONSOLE_PRINT("networkcoms[1]: TCPIP Stack is Ready on %s Interface\r\n",
									  TCPIP_STACK_NetNameGet(netwData.networkInterfaceHandle));
					SYS_CONSOLE_PRINT("networkcoms[1]: Host name is %s\r\n",
									  TCPIP_STACK_NetBIOSName(netwData.networkInterfaceHandle));
					SYS_CONSOLE_PRINT("networkcoms[1]: DHCP Client is enabled on %s interface\r\n",
									  TCPIP_STACK_NetNameGet(netwData.networkInterfaceHandle));

					netwData.timeoutTick = SYS_TMR_TickCountGet();
					netwData.state = NETWORKCOMS_STATE_WAIT_FOR_LINK;
				}
			}
			break;

		case NETWORKCOMS_STATE_WAIT_FOR_LINK:
			//Determine if the network interface is connected
			if (TCPIP_STACK_NetIsLinked(netwData.networkInterfaceHandle))
			{
				if (netwData.consolePrintOnceFlag == false)
				{
					SYS_CONSOLE_MESSAGE("networkcoms[2]: Network Link is Up\r\n");
					netwData.consolePrintOnceFlag = true;
				}

				//Check if the DHCP Server has assigned an IP Address
				if (TCPIP_DHCP_IsBound(netwData.networkInterfaceHandle))
				{
					netwData.hostIPAddress.Val = TCPIP_STACK_NetAddress(netwData.networkInterfaceHandle);
					SYS_CONSOLE_PRINT("networkcoms[2]: DHCP Client has received IP Address: %d.%d.%d.%d\r\n",
									  netwData.hostIPAddress.v[0], netwData.hostIPAddress.v[1], netwData.hostIPAddress.v[2], netwData.hostIPAddress.v[3]);
					netwData.state = NETWORKCOMS_SETUP_UDP_SERVER;
				}
				else if (SYS_TMR_TickCountGet() - netwData.timeoutTick >=
						 SYS_TMR_TickCounterFrequencyGet() * DHCP_DETECTION_TIMEOUT_PERIOD_s)
				{
					//If no DHCP Server is found on the network after the timeout
					//period defined by DHCP_DETECTION_TIMEOUT_PERIOD_s
					//the PIC will disable the DHCP Client, and it
					//will become a DHCP Server.

					SYS_CONSOLE_MESSAGE("networkcoms[2]: DHCP Server Detection Timeout\r\n");

					//Disable DHCP Client
					TCPIP_DHCP_Disable(netwData.networkInterfaceHandle);
					SYS_CONSOLE_PRINT("networkcoms[2]: DHCP Client is disabled on %s interface\r\n",
									  TCPIP_STACK_NetNameGet(netwData.networkInterfaceHandle));

					//Enable DHCP Server
					TCPIP_DHCPS_Enable(netwData.networkInterfaceHandle);
					SYS_CONSOLE_PRINT("networkcoms[2]: DHCP Server is active on %s interface\r\n",
									  TCPIP_STACK_NetNameGet(netwData.networkInterfaceHandle));

					netwData.state = NETWORKCOMS_SETUP_UDP_SERVER;
				}
			}
			else
			{
				if (netwData.networkLinkDropFlag == true)
				{
					//When the network link drops, disable the DHCP Server
					//and re-enable the DHCP Client
					netwData.networkLinkDropFlag = false;

					//Since the DHCP Server is configured for only 1 entry in the pool
					//the existing entry is removed to allow a different device
					//to connect to the PIC and obtain an IP Address
					TCPIP_DHCPS_RemovePoolEntries(netwData.networkInterfaceHandle, DHCP_SERVER_POOL_ENTRY_ALL);

					//Disable DHCP Server
					TCPIP_DHCPS_Disable(netwData.networkInterfaceHandle);

					//Enable DHCP Client
					TCPIP_DHCP_Enable(netwData.networkInterfaceHandle);

					SYS_CONSOLE_PRINT("networkcoms[2]: DHCP Client is enabled on %s interface\r\n",
									  TCPIP_STACK_NetNameGet(netwData.networkInterfaceHandle));
				}
				netwData.timeoutTick = SYS_TMR_TickCountGet();
			}
			break;

		case NETWORKCOMS_SETUP_UDP_SERVER:
			/* TODO:[1] Open a UDP Server Instance
			 * - IPv4 address type
			 * - Port is defined in the ECS_UDP_BROADCAST_PORT constant
			 * - Accept connections on ANY interface.
			 * - Store returned handle in the netwData.ecsUDPBroadcastHandle variable*/
			netwData.ecsUDPBroadcastHandle = TCPIP_UDP_ServerOpen(IP_ADDRESS_TYPE_IPV4,ECS_UDP_BROADCAST_PORT,0);//<--Insert the solution for item 1 on this line
			if (netwData.ecsUDPBroadcastHandle != INVALID_UDP_SOCKET)
			{
				SYS_CONSOLE_MESSAGE("networkcoms[3]: UDP Server Opened\r\n");
				netwData.state = NETWORKCOMS_LISTEN_FOR_ECS_BROADCAST;
			}
			else
			{
				SYS_CONSOLE_MESSAGE("networkcoms[3] Error Opening UDP Server\r\n");
				netwData.state = NETWORKCOMS_ERROR;
			}
			break;

		case NETWORKCOMS_LISTEN_FOR_ECS_BROADCAST:
			/*TODO:[2] Check if any data has been received on the UDP Socket
             */
			if (TCPIP_UDP_GetIsReady(netwData.ecsUDPBroadcastHandle) > 0) //<--Insert the solution for item 2 on this line (replace the underscores with the solution)
			{
				// Copy the data received on the UDP Socket into the receivedDataBuffer.
				// The numberDataBytesToProcess returned value will be the lesser of
				// the size of the receivedDataBuffer Array-1 or
				// the number of bytes that has been received.
				numberDataBytesToProcess = TCPIP_UDP_ArrayGet(netwData.ecsUDPBroadcastHandle, receivedDataBuffer, sizeof (receivedDataBuffer - 1));

				receivedDataBuffer[numberDataBytesToProcess] = '\0';
				SYS_CONSOLE_PRINT("networkcoms[4]: Data Received on UDP Socket: %s\r\n", receivedDataBuffer);

				//Process the received data packet
				while (ECSDATAPROCESSING_Parse(&netwData.ecsParseContext, receivedDataBuffer, numberDataBytesToProcess, &netwData.elevatorStatusData))
				{
					//Check if the received data packet was a broadcast
					if (netwData.ecsParseContext.parsedPacketType == ECS_BROADCAST)
					{
						if (TCPIP_UDP_SocketInfoGet(netwData.ecsUDPBroadcastHandle,&netwData.ecsUDPSocketInfo))
						{
							SYS_CONSOLE_PRINT("networkcoms[4]: ECS Broadcast found on IP %d.%d.%d.%d\r\n",
											  netwData.ecsUDPSocketInfo.sourceIPaddress.v4Add.v[0],
											  netwData.ecsUDPSocketInfo.sourceIPaddress.v4Add.v[1],
											  netwData.ecsUDPSocketInfo.sourceIPaddress.v4Add.v[2],
											  netwData.ecsUDPSocketInfo.sourceIPaddress.v4Add.v[3]);

							/*TODO:[3] Close the UDP Server Instance
                             */
							TCPIP_UDP_Close(netwData.ecsUDPBroadcastHandle);//<--Insert the solution for item 3 on this line

							//Goto the Setup TCP Client State
							SYS_CONSOLE_MESSAGE("networkcoms[4]: UDP Server Closed\r\n");
							netwData.ecsUDPBroadcastHandle = INVALID_UDP_SOCKET;
							netwData.state = NETWORKCOMS_SETUP_TCP_CLIENT;
						}
					}
				}
			}
			break;

		case NETWORKCOMS_SETUP_TCP_CLIENT:
			/* TODO:[4] Open a TCP Client Instance
			 * - IPv4 Address Type
			 * - Destination Port is defined in ECS_TCP_PORT constant
			 * - IP Address is stored in netwData.ecsUDPSocketInfo.sourceIPaddress structure
			 * - Store returned handle in the netwData.ecsTCPSocketHandle variable.
			 * Hint: You must pass the address of netwData.ecsUDPSocketInfo.sourceIPaddress 
			 * to the Harmony API that is used to open the TCP Client.*/
			netwData.ecsTCPSocketHandle = TCPIP_TCP_ClientOpen(IP_ADDRESS_TYPE_IPV4,ECS_TCP_PORT,&netwData.ecsUDPSocketInfo.sourceIPaddress);//<--Insert the solution for item 4 on this line
			if (netwData.ecsTCPSocketHandle != INVALID_SOCKET)
			{
				SYS_CONSOLE_MESSAGE("networkcoms[5]: TCP Client Opened\r\n");
				netwData.timeoutTick = SYS_TMR_TickCountGet();
				netwData.state = NETWORKCOMS_WAIT_FOR_TCP_CONNECTION;
			}
			else
			{
				SYS_CONSOLE_MESSAGE("networkcoms[5]: Error Opening TCP Client\r\n");
				netwData.state = NETWORKCOMS_ERROR;
			}
			break;

		case NETWORKCOMS_WAIT_FOR_TCP_CONNECTION:
			/*TODO:[5] Check if the TCP Client is connected to the Server 
             */
			if (TCPIP_TCP_IsConnected(netwData.ecsTCPSocketHandle)) //<--Insert the solution for item 5 on this line (replace the underscores with the solution)
			{
				SYS_CONSOLE_MESSAGE("networkcoms[6]: TCP Socket Connected\r\n");

				//Clear the TCP Connection Reset Semaphore
				TCPIP_TCP_WasReset(netwData.ecsTCPSocketHandle);

				//Load start tick to allow the next 500ms period to be timed
				netwData.statusRequestTick = SYS_TMR_TickCountGet();
				netwData.consolePrintOnceFlag = false;
				netwData.callRequestState = CALLREQ_CHECK_FOR_NEW_REQUEST;
				netwData.statusRequestState = STATUSREQ_WAIT_FOR_TIMER;
				netwData.state = NETWORKCOMS_MANAGE_TCP_CLIENT;
			}
			else
			{
				//If the TCP Connection is not established after a period
				//defined by TCP_CLIENT_CONNECTION_TIMEOUT_PERIOD_s, the socket
				//will be closed.
				if (SYS_TMR_TickCountGet() - netwData.timeoutTick >=
					SYS_TMR_TickCounterFrequencyGet() * TCP_CLIENT_CONNECTION_TIMEOUT_PERIOD_s)
				{
					SYS_CONSOLE_MESSAGE("networkcoms[6]: TCP Socket could not be established\r\n");
					netwData.state = NETWORKCOMS_CLOSE_TCP_CLIENT;
				}
			}
			break;

		case NETWORKCOMS_MANAGE_TCP_CLIENT:
			/* TODO:[6] Check if the TCP connection has dropped out 
			 * any time between the current and previous execution 
			 * of the NETWORKCOMS_MANAGE_TCP_CLIENT state.*/
			if (TCPIP_TCP_WasReset(netwData.ecsTCPSocketHandle)) //<--Insert the solution to item 6 on this line (replace the underscores)
			{
				SYS_CONSOLE_MESSAGE("networkcoms[7]: TCP Socket Disconnected\r\n");
				//If the connection has dropped out, close the TCP Socket
				netwData.state = NETWORKCOMS_CLOSE_TCP_CLIENT;
			}
			else
			{
				//Data Reception Management------------------------------------------------------------------------------------------------
				//Check if any data has been received on the socket
				if (TCPIP_TCP_GetIsReady(netwData.ecsTCPSocketHandle) > 0)
				{
					//Copy the received data into the receivedDataBuffer.
					numberDataBytesToProcess = TCPIP_TCP_ArrayGet(netwData.ecsTCPSocketHandle, receivedDataBuffer, sizeof (receivedDataBuffer));

					//ECS_DATAPROCESSING_Parse will return with TRUE when there is a complete parsed data packet available.
					//There still may be unprocessed data in the receivedDataBuffer, hence the Parse function is
					//continuously called until there is no more complete data packets.
					while (ECSDATAPROCESSING_Parse(&netwData.ecsParseContext, receivedDataBuffer,
												   numberDataBytesToProcess, &netwData.elevatorStatusData))
					{
						if (netwData.ecsParseContext.parsedPacketType == ECS_RESPONSE_CALLACK &&
							netwData.callRequestState == CALLREQ_WAIT_FOR_RESPONSE)
						{
							netwData.callRequestState = CALLREQ_CHECK_FOR_NEW_REQUEST;
							SYS_CONSOLE_MESSAGE("networkcoms[7]: Received Call Acknowledgement from ECS \r\n");
						}
						else if (netwData.ecsParseContext.parsedPacketType == ECS_RESPONSE_STATUS &&
								 netwData.statusRequestState == STATUSREQ_WAIT_FOR_RESPONSE)
						{
							netwData.statusRequestState = STATUSREQ_REARM_TIMER;
						}
					}
				}
				//-------------------------------------------------------------------------------------------------------------------------

				//Management of the Call Request-------------------------------------------------------------------------------------------
				switch (netwData.callRequestState)
				{
					case CALLREQ_CHECK_FOR_NEW_REQUEST:
						//Check if any of the call buttons have been pressed
						if (BUTTONCONTROL_GetCallButtonState() > ECS_CALL_NONE)
						{
							netwData.callRequestState = CALLREQ_TRANSMIT_REQUEST;
							SYS_CONSOLE_PRINT("networkcoms[7]: Call Button %d Pressed\r\n",
											  (uint8_t) BUTTONCONTROL_GetCallButtonState());
						}
						break;

					case CALLREQ_TRANSMIT_REQUEST:
						/* TODO:[7] Get the amount of free space
						 * available in the Transmit Buffer of the TCP Socket */
						if (TCPIP_TCP_PutIsReady(netwData.ecsTCPSocketHandle) >= strlen(callReq_JSONPacket)) //<--Insert the solution to item 7 on this line (replace the underscores with the solution)
						{
							//Modify the "call source" data value in the JSON packet
							callReq_JSONPacket[27] = (uint8_t) BUTTONCONTROL_GetCallButtonState() + '0';

							//Clear the call request latches to prevent multiple
							//call requests from being sent while the button is pressed
							BUTTONCONTROL_ClearCallButtonLatches();

							//Load a new tick which is used for a timeout on the reception of the response
							netwData.timeoutTick = SYS_TMR_TickCountGet();

							/*TODO:[8] Send the string stored in callReq_JSONPacket array to the TCP Server
                             */
							TCPIP_TCP_ArrayPut(netwData.ecsTCPSocketHandle, callReq_JSONPacket, strlen(callReq_JSONPacket));//<--Insert the solution to item 8 on this line
							SYS_CONSOLE_MESSAGE("networkcoms[7]: Call Request Transmitted to ECS\r\n");

							netwData.callRequestState = CALLREQ_WAIT_FOR_RESPONSE;
						}
						break;

					case CALLREQ_WAIT_FOR_RESPONSE:
						//When an acknowledgment is received the callRequestState will
						//change to CALLREQ_CHECK_FOR_NEW_REQUEST (this is performed by the data reception procssing code)

						//Check if a response has been received within a time window
						//defined by ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s
						if (SYS_TMR_TickCountGet() - netwData.timeoutTick >=
							SYS_TMR_TickCounterFrequencyGet() * ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s)
						{
							SYS_CONSOLE_MESSAGE("networkcoms[7]: Call Request: Response Timeout\r\n");
							//If no response has been received, close the TCP Socket
							netwData.state = NETWORKCOMS_CLOSE_TCP_CLIENT;
						}
						break;

					default:
						netwData.callRequestState = CALLREQ_CHECK_FOR_NEW_REQUEST;
						break;
				}
				//-------------------------------------------------------------------------------------------------------------------------

				//Management of the Status Request-----------------------------------------------------------------------------------------
				switch (netwData.statusRequestState)
				{
					case STATUSREQ_WAIT_FOR_TIMER:
						//Check if 500ms has expired
						if (SYS_TMR_TickCountGet() - netwData.statusRequestTick >=
							SYS_TMR_TickCounterFrequencyGet() / ECS_STATUS_REQUEST_POLL_FREQUENCY_Hz)
						{
							netwData.statusRequestState = STATUSREQ_TRANSMIT_REQUEST;
						}
						break;

					case STATUSREQ_TRANSMIT_REQUEST:
						if (TCPIP_TCP_PutIsReady(netwData.ecsTCPSocketHandle) >= strlen(statusReq_JSONPacket))
						{
							TCPIP_TCP_StringPut(netwData.ecsTCPSocketHandle, statusReq_JSONPacket);

							//Load a new tick which is used for a timeout on the reception of the response
							netwData.statusRequestTick = SYS_TMR_TickCountGet();

							netwData.statusRequestState = STATUSREQ_WAIT_FOR_RESPONSE;
						}
						break;

					case STATUSREQ_WAIT_FOR_RESPONSE:
						//Once the status response is received, the statusRequestState will
						//change to STAT_REQ_REARM_TIMER (this is performed by the data reception procssing code)

						//Check if a response has been received within a time window
						//defined by ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s
						if (SYS_TMR_TickCountGet() - netwData.statusRequestTick >=
							SYS_TMR_TickCounterFrequencyGet() * ECS_SERVER_RESPONSE_TIMEOUT_PERIOD_s)
						{
							SYS_CONSOLE_MESSAGE("networkcoms[7]: Status Request: Response Timeout\r\n");

							//If no response has been received, close the TCP Socket
							netwData.state = NETWORKCOMS_CLOSE_TCP_CLIENT;
						}
						break;

					case STATUSREQ_REARM_TIMER:
						//Reload start tick to allow the next 500ms period to be timed
						netwData.statusRequestTick = SYS_TMR_TickCountGet();

						//Update the call indicator lamps
						LEDCONTROL_SetCallIndicators(netwData.elevatorStatusData.callindicators);

						if (netwData.consolePrintOnceFlag == false)
						{
							SYS_CONSOLE_PRINT("networkcoms[7]: ECS Assignment: Elevator: %d, Floor: %d\r\n",
											  netwData.elevatorStatusData.carID, netwData.elevatorStatusData.floorID);

							netwData.consolePrintOnceFlag = true;
						}

						netwData.statusRequestState = STATUSREQ_WAIT_FOR_TIMER;
						break;

					default:
						netwData.statusRequestState = STATUSREQ_REARM_TIMER;
						break;
				}
				//-------------------------------------------------------------------------------------------------------------------------
			}
			break;

		case NETWORKCOMS_CLOSE_TCP_CLIENT:
			TCPIP_TCP_Close(netwData.ecsTCPSocketHandle);
			netwData.ecsTCPSocketHandle = INVALID_SOCKET;

			SYS_CONSOLE_MESSAGE("networkcoms[8]: TCP Client Closed\r\n");
			netwData.state = NETWORKCOMS_SETUP_UDP_SERVER;
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