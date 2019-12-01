#ifndef SYSTEM_CONFIG_H
#define SYSTEM_CONFIG_H

// avoid superfluous warnings when building harmony
#define _PLIB_UNSUPPORTED

/* Clock System Service Configuration */
#define SYS_CLK_FREQ                        200000000ul
#define SYS_CLK_BUS_PERIPHERAL_1            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_2            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_3            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_4            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_5            100000000ul
#define SYS_CLK_BUS_PERIPHERAL_7            200000000ul
#define SYS_CLK_BUS_PERIPHERAL_8            100000000ul
#define SYS_CLK_CONFIG_PRIMARY_XTAL         24000000ul
#define SYS_CLK_CONFIG_SECONDARY_XTAL       32768ul

/*** Console System Service Configuration ***/
#define SYS_CONSOLE_OVERRIDE_STDIO
#define SYS_CONSOLE_DEVICE_MAX_INSTANCES        2
#define SYS_CONSOLE_INSTANCES_NUMBER            1
#define SYS_CONSOLE_USB_CDC_INSTANCE       		USB_DEVICE_CDC_INDEX_0
#define SYS_CONSOLE_USB_CDC_COMM_BAUD_RATE 		921600
#define SYS_CONSOLE_USB_CDC_RD_QUEUE_DEPTH 		1
#define SYS_CONSOLE_USB_CDC_WR_QUEUE_DEPTH 		128
#define SYS_CONSOLE_USB_CDC_READ_BUFFER_SIZE   	512
#define SYS_CONSOLE_BUFFER_DMA_READY __attribute__((coherent)) __attribute__((aligned(16)))

/*** Interrupt System Service Configuration ***/
#define SYS_INT                     true

/*** Timer System Service Configuration ***/
#define SYS_TMR_POWER_STATE             SYS_MODULE_POWER_RUN_FULL
#define SYS_TMR_DRIVER_INDEX            DRV_TMR_INDEX_0
#define SYS_TMR_MAX_CLIENT_OBJECTS      5
#define SYS_TMR_FREQUENCY               1000
#define SYS_TMR_FREQUENCY_TOLERANCE     10
#define SYS_TMR_UNIT_RESOLUTION         10000
#define SYS_TMR_CLIENT_TOLERANCE        10
#define SYS_TMR_INTERRUPT_NOTIFICATION  false

/*** Timer Driver Configuration ***/
#define DRV_TMR_INTERRUPT_MODE             	true
#define DRV_TMR_INSTANCES_NUMBER           	1
#define DRV_TMR_CLIENTS_NUMBER             	1

/*** Timer Driver 0 Configuration ***/
#define DRV_TMR_PERIPHERAL_ID_IDX0          TMR_ID_1
#define DRV_TMR_INTERRUPT_SOURCE_IDX0       INT_SOURCE_TIMER_1
#define DRV_TMR_INTERRUPT_VECTOR_IDX0       INT_VECTOR_T1
#define DRV_TMR_ISR_VECTOR_IDX0             _TIMER_1_VECTOR
#define DRV_TMR_INTERRUPT_PRIORITY_IDX0     INT_PRIORITY_LEVEL1
#define DRV_TMR_INTERRUPT_SUB_PRIORITY_IDX0 INT_SUBPRIORITY_LEVEL0
#define DRV_TMR_CLOCK_SOURCE_IDX0           DRV_TMR_CLKSOURCE_INTERNAL
#define DRV_TMR_PRESCALE_IDX0               TMR_PRESCALE_VALUE_256
#define DRV_TMR_OPERATION_MODE_IDX0         DRV_TMR_OPERATION_MODE_16_BIT
#define DRV_TMR_ASYNC_WRITE_ENABLE_IDX0     false
#define DRV_TMR_POWER_STATE_IDX0            SYS_MODULE_POWER_RUN_FULL

// *****************************************************************************
// *****************************************************************************
// Section: USB Configuration
// *****************************************************************************
// *****************************************************************************

/*** USB Driver Configuration ***/

// work as a USB device not as a host
#define DRV_USB_DEVICE_SUPPORT      true
#define DRV_USB_HOST_SUPPORT        false

/* Enables Device Support */
#define DRV_USBHS_DEVICE_SUPPORT      true
#define DRV_USBHS_HOST_SUPPORT        false

/* Maximum USB driver instances */
#define DRV_USBHS_INSTANCES_NUMBER    1

/* Interrupt mode enabled */
#define DRV_USBHS_INTERRUPT_MODE      true

/* Number of Endpoints used */
#define DRV_USBHS_ENDPOINTS_NUMBER    3

/*** USB Device Stack Configuration ***/

/* The USB Device Layer will not initialize the USB Driver */
#define USB_DEVICE_DRIVER_INITIALIZE_EXPLICIT

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER         1

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE          64

/* Maximum instances of CDC function driver */
#define USB_DEVICE_CDC_INSTANCES_NUMBER     1

/* Total size of the CDC read and write queues */
#define USB_DEVICE_CDC_QUEUE_DEPTH_COMBINED 3

#endif 
