** Harmony Middleware **

USB
	HID example (section 20.5)

CDC
	Inspired from the HID example (semi Harmony)

NET1
	Introduction to the TCP/IP Stack (21070_NET1)

common libraries (cdc, hid, tcp) with the same api:
	- xxx_setup() initialize the device 
	- xxx_open() attempt to open the device, return true when ...
	- xxx_update() call from the main loopupdate the necessary harmony state machines, 