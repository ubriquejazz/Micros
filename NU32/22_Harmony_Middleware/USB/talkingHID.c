/*!\name      talkingHID.h
 *
 * \brief     Echo example.
 *
 * \author    Juan Gago
 *
 */

#include "hid.h"
#include "system_config.h"
#include "system_definitions.h"
#include <sys/attribs.h>
#include <ctype.h> //for toupper

#define REPORT_LEN 0x40 // reports have 64 bytes in them

// the HID report descriptor (see Universal Serial Bus HID Usage Tables document).
// This descriptor contains input and output reports that are 64 bytes long.  The
// data can be anything. Borrowed from harmony hid_basic example.
const uint8_t HID_REPORT[NU32_REPORT_SIZE] = {
  0x06, 0x00, 0xFF,   // Usage Page = 0xFF00 (Vendor Defined Page 1)
  0x09, 0x01,         // Usage (Vendor Usage 1)
  0xA1, 0x01,         // Collection (Application)
  0x19, 0x01,         // Usage Minimum
  0x29, REPORT_LEN,   // Usage Maximum  64 input usages total (0x01 to 0x40)
  0x15, 0x01,         // Logical Minimum (data bytes in the report have min value = 0x00)
  0x25, 0x40,         // Logical Maximum (data bytes in the report have max value = 0xFF)
  0x75, 0x08,         // Report Size: 8-bit field size
  0x95, REPORT_LEN,   // Report Count: 64 8-bit fields 
                      //    (for next "Input", "Output", or "Feature" item)
  0x81, 0x00,         // Input (Data, Array, Abs): input packet fields 
                      // ased on the above report size, count, logical min/max, and usage
  0x19, 0x01,         // Usage Minimum
  0x29, REPORT_LEN,   // Usage Maximum  64 output usages total (0x01 to 0x40)
  0x91, 0x00,         // Output (Data, Array, Abs): Instantiates output packet fields.  
                      // Uses same report size and count as "Input" fields, since nothing 
                      // new/different was specified to the parser since the "Input" item.
  0xC0                // End Collection
};

// states for the application
typedef enum {APP_STATE_INIT, APP_STATE_RECEIVE, APP_STATE_SEND} APP_STATE;  

bool to_upper(char * curr) { // convert to upper case
    while(*curr) { 
    *curr = toupper(*curr);
    ++curr;
  }
  return true;
}

int main (void) 
{
  APP_STATE state = APP_STATE_INIT;
  char report[REPORT_LEN]=""; // message report buffer 64 bytes per hid report descriptor

  hid_setup();                // initialize the hid usb helper module
  PLIB_INT_Enable(INT_ID_0);  // enable interrupts

  while (1) 
  {
    switch(state) 
    {
      case APP_STATE_INIT:
        if(hid_open()) {                  // wait for the hid device to open
          state = APP_STATE_RECEIVE;
        }
        break;

      case APP_STATE_RECEIVE:
        if(hid_receive((unsigned char *)report, REPORT_LEN)) { // we are finished receiving 
          to_upper(report);       // convert to upper case
          state = APP_STATE_SEND; // send data to client
        }
        break;

      case APP_STATE_SEND:
        if(hid_send((unsigned char *)report, REPORT_LEN) ) { // finished sending
          state = APP_STATE_RECEIVE;                         // receive data again
        }
        break;

      default:
        break; // logic error, impossible state!
    }
    hid_update();     // update the usb hid state
  }
  return 0;
}
