#ifndef __ENCAPSULATIONROUTING
#define __ENCAPSULATIONROUTING

/*!\name      EncapsulationRouting.c
 *
 * \brief     This file provides the routing facilities within the 
 *            system.
 *
 */

#include "mrtsTypes.h"
#include "EncapsulationProtocol.h"
#include "EncapsulationProcessor.h"

typedef int32_t (DestinationProcessor_t )    (BaseObject_t*            buffer,
                                              size_t                   packetSize,
                                              PacketType_t             type,
                                              EncAddr_t                src,
                                              EncAddr_t                dst,
                                              uint32_t                 dataIndex,
                                              size_t                   dataLength,
                                              bool                     crcOk
                                             );

typedef struct EncapsulationRouteEntry_t
{
   EncAddr_t               dst;
   RxdAddressedPacketCb_t* addressedProcessor;
   void*                   assocData;
} EncapsulationRouteEntry_t;


typedef struct EncapsulationRoutingTable_t
{

   BaseObject_t               base;
   EncAddr_t                  localSubnet;
   int32_t                    numEntries;
   bool                       mallocedEntries;
   EncapsulationRouteEntry_t* entries;
} EncapsulationRoutingTable_t;


int32_t EncapsulationRouting_Init ( BaseObject_t* owner, EncAddr_t subnet, EncapsulationRoutingTable_t* table, uint32_t numRoutes, EncapsulationRouteEntry_t* routes);

int32_t EncapsulationRouting_Teardown( EncapsulationRoutingTable_t* table );

/*! \name     EncapsulationRouting_AddRoute
 *
 *  \brief    This function adds a route to the table for a specific 
 *            destination. If the destination already has an entry it is
 *            overwritten.
 *
 * \return    0 - not added/found otherwise a pointer to the entry.
 */           
EncapsulationRouteEntry_t* EncapsulationRouting_AddRoute( EncapsulationRoutingTable_t* table, EncapsulationRouteEntry_t* newEntry );


/*! \name     EncapsulationRouting_FindEntry
 *
 *  \brief    Find an entry in the routing table.
 *
 * \return    0 - not found (for any reason) otherwise a pointer to the entry.
 */
EncapsulationRouteEntry_t* EncapsulationRouting_FindEntry( EncapsulationRoutingTable_t* table, EncAddr_t dst );


#endif

