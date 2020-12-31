# BQ76952 Battery Monitoring

== Applications

commands	Examples for the different BQ769x2 command types. The program pauses 
			after each command and waits for the user to press the P2.3 button to 
			execute the next command. This allows bus capture on a logic analyzer.

system_app	This example is using the BQ76942 for a 7-cell battery. The ALERT 
			pin of the BQ76942 will operate as an interrupt for the MCU to notify 
			that measurements are ready or faults are present. 
		
== Libraries

i2c.{c,h}	It's equivalent to system_interrupt
init.c		It's equivalent to system_init
bq.{c,h}	It's based on i2c.h driver