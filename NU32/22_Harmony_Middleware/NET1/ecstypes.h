#ifndef ECSTYPES_H
#define	ECSTYPES_H

/*!\name      ecstypes.h
 *
 * \brief     Elevator Control Server (Raspberry Pi2)
 *            Elevator or Landing Call System (PIC32)
 *
 * \author    Juan Gago
 *
 */

typedef enum
{
    ECS_CALL_NONE = 0,
    ECS_CALL_DOWN = 1,
    ECS_CALL_UP   = 2,
    ECS_CALL_BOTH = 3
} ECS_CALL_BUTTON_STATE_TYPE;
 
typedef enum
{
    DIR_NONE = 0,
    DIR_DOWN = 1,
    DIR_UP   = 2
} ELEVATOR_DIRECTION_TYPE;
 
typedef struct
{
    uint16_t carID;
    uint16_t floorID;
    uint16_t floorPos;
    ELEVATOR_DIRECTION_TYPE     direction;
    ECS_CALL_BUTTON_STATE_TYPE  callindicators;
} ELEVATOR_STATUS_TYPE;

#endif