# This automatically compiles and links every .c file in the directory into a single 
# executable, your project should contain all the C files you need, nothing else!

# XC32PATH is the path to the xc32 bin directory. This directory contains
# tools such as the compiler (xc32-gcc) and hex converter (xc32-bin2hex)
# HARMONYPATH is the path to the harmony framework directory
# NU32PATH is the path to the nu32utility. You compiled this and named it nu32utility
# PORT is the COM port used by the PIC32. Windows Example: PORT=\\.\COM22
# TERMEMU is your terminal emulator
# Windows example: TERMEMU=C:/Program Files (x86)/PuTTY/putty.exe

XC32PATH=/Applications/microchip/xc32/v2.15/bin
HARMONYPATH=/Users/juan/microchip/harmony/v2_06
NU32PATH=~/PycharmProjects/Kinder
PORT=/dev/tty.usbserial
TERMEMU=screen
PROC = 32MX795F512H
TARGET=out

# We try to detect the OS
RM = rm -f
MKDIR = mkdir -p

#the c compiler
CC="$(XC32PATH)/xc32-gcc"

#the hexfile maker
HX="$(XC32PATH)/xc32-bin2hex"

#object dumper
OBJDMP="$(XC32PATH)/xc32-objdump"

#the bootloader utility
WRITE="$(NU32PATH)/nu32utility"

#the linker script
LINKSCRIPT=NU32bootloaded.ld

#additional linker flags
LINKFLAGS=-Map=$(TARGET).map

#if we have specified a linker script add it
ifdef LINKSCRIPT
	LINKFLAGS:=--script=$(LINKSCRIPT),$(LINKFLAGS)
endif

#get all the c files in the system_config directory
SYSFILES = 

#strip filenames from the files used so we add those subdirs to the include path
#also add -I before each filename
INCPATH=$(patsubst %,-I"%",$(dir $(SYSFILES)))

OBJS := $(patsubst %.c, %.o,$(wildcard *.c))
HDRS := $(wildcard *.h)
CFLAGS=-g -O1 -x c

#what to do when make all
.PHONY : all
all : $(TARGET).hex $(TARGET).dis

# Turn the elf file into a hex file.
$(TARGET).hex : $(TARGET).elf
	@echo Creating hex file
	$(HX) $(TARGET).elf

# Perform disassembly
$(TARGET).dis : $(TARGET).elf
	$(OBJDMP) -S $< > $@

# Link all the object files into an elf file.
$(TARGET).elf : $(OBJS)
	@echo Linking elf file
	$(CC) -mprocessor=$(PROC) -o $(TARGET).elf $(OBJS) -Wl,$(LINKFLAGS)

# Create an object file for each C file.
%.o : %.c $(HDRS)
	@echo Creating object file $@
	$(CC) $(CFLAGS) -c -mprocessor=$(PROC) -o $@ $<
	
.PHONY: clean
# Erase all hex, map, object, and elf files.
clean :
	$(RM) *.hex *.map *.o *.elf *.dep *.dis       

.PHONY: write
# After making, call the NU32utility to program via bootloader.
write : $(TARGET).hex $(TARGET).dis
	$(WRITE) "$(PORT)" $(TARGET).hex 

.PHONY: putty
putty :
	"$(TERMEMU)" -serial -sercfg 230400,R "$(PORT)"

.PHONY: screen
screen :
	"$(TERMEMU)" "$(PORT)" 230400,crtscts
