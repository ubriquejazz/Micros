/*!\name      EncapsulationRouting.c
 *
 * \brief     This file provides the routing facilities within the 
 *            system.
 *
 */

#include <stdio.h>
#include <string.h>

#include "mrtsErrors.h"
#include "EncapsulationRouting.h"


/* Define RELSTATIC in the makefile to static to make this go away */
#ifndef RELSTATIC
   #define RELSTATIC
#endif

/*!\name      EncapsulationRouting_Init
 *
 * \brief     Setup the routing table.
 *
 * \param     owner          The pointer to base object that contains this
 *                           routing
 */

int32_t EncapsulationRouting_Init( BaseObject_t* owner, EncAddr_t subnet, EncapsulationRoutingTable_t* table, uint32_t numRoutes, EncapsulationRouteEntry_t* routes)
{
   if ( owner == NULL  ||  NULL == table)
   {
      return merNullPointer;
   }

   if (ENC_GET_ADDRESS(subnet))
   {
      return merInvalidSubnet;
   }

   table->base.self = (BaseObject_t*) table;
   table->base.owner = owner;
   table->localSubnet = subnet;
   table->numEntries = 0;
   if ( 0 == numRoutes )
   {
      return merRangeExceeded;
   }

   table->mallocedEntries = false;
   if ( NULL==routes )
   {
      table->entries = calloc( numRoutes, sizeof ( EncapsulationRouteEntry_t ));
      if ( NULL==table->entries )
      {
         return merNullPointer;
      }
      table->mallocedEntries = true;
   }
   else
   {
      table->entries = routes;
   }

   table->numEntries = numRoutes;

   return table->numEntries;
}


int32_t EncapsulationRouting_Teardown( EncapsulationRoutingTable_t* table )
{
   if ( NULL==table)
   {
      return merNullPointer;
   }

   if ( true == table->mallocedEntries )
   {
      free( table->entries );
   }
   memset( table, 0, sizeof(EncapsulationRoutingTable_t));

   return 0;
}


EncapsulationRouteEntry_t* EncapsulationRouting_AddRoute( EncapsulationRoutingTable_t* table, EncapsulationRouteEntry_t* newEntry )
{
   EncapsulationRouteEntry_t* entry = NULL;

   /* Look for existing entry for this route */
   if (newEntry)
   {
      EncAddr_t filteredDest = (ENC_GET_SUBNET(table->localSubnet)==ENC_GET_SUBNET(newEntry->dst)) ? ENC_GET_ADDRESS(newEntry->dst) : ENC_GET_SUBNET(newEntry->dst);
      DPRINTF("\n\r%s dest=%04x filtered=%04x\n\r", __PRETTY_FUNCTION__, newEntry->dst, filteredDest);

      entry = EncapsulationRouting_FindEntry( table, newEntry->dst );

      if (!entry)
      {
          int i=0;
         /* Find free space */
         for (i=0; i<table->numEntries && NULL==entry; i++)
         {
            DPRINTF("Looking %d", i);
            if (NULL == table->entries[i].addressedProcessor)
            {
               entry = &(table->entries[i]);
            }
         }
      }

      if (entry)
      {
         *entry = *newEntry;
         entry->dst = filteredDest;
      }
   }
   return entry; 
}


/*!\name      EncapsulationRouting_FindEntry
 *
 * \brief     This function is used to find a destination in the routing table:
 *
 * \param     table          The routing table - which must have already been
 *                           initialised.
 *
 * \param     dst
 * \parblock
 *                           The address of the device or subnet we are looking
 *                           for - it is checked to see if it is aimed at a
 *                           local device or one outside this subnet as below:
 *
 *                           If the subnet of the address matches that of this
 *                           table it is discarded: The device has to be local
 *                           and this makes it easier to reuse the same blocks
 *                           in a repeated form (IrDA ports, blades etc.)
 *
 *                           If the subnet of the address doesn't match that of
 *                           the table it means we need to route outside the
 *                           subnet so the local address is irrelevant and we
 *                           can discard it. This means we only need to store
 *                           routes to subnets and local devices.
 * \endparblock
 *
 * \returns                  If the appropriate route is found it's
 *                           corresponding entry in the routing table is
 *                           returned, otherwise NULL is returned.
 */
EncapsulationRouteEntry_t* EncapsulationRouting_FindEntry( EncapsulationRoutingTable_t* table, EncAddr_t dst )
{
   if (NULL==table)
   {
      return NULL;
   }

   EncapsulationRouteEntry_t* route = NULL;
   int i=0;
   EncAddr_t filteredDest = (ENC_GET_SUBNET(table->localSubnet)==ENC_GET_SUBNET(dst)) ? ENC_GET_ADDRESS(dst) : ENC_GET_SUBNET(dst);

   for (i=0; i<table->numEntries && NULL==route; i++)
   {
      if (filteredDest == table->entries[i].dst)
      {
         if (table->entries[i].addressedProcessor)
         {
            route = &(table->entries[i]);
         }
      } 
   }

   return route;
}



