# We try to detect the OS
CLEANUP = rm -f
MKDIR = mkdir -p

SRC_DIR=./src
OBJ_DIR=./obj
PROJECT_ROOT=../../../..

TARGETLIB = $(OBJ_DIR)/libtesters.a
UNITY_ROOT= $(PROJECT_ROOT)/Unity
INC_DIRS  = -I$(UNITY_ROOT)/src
SYMBOLS   = -DMOCK -DVERBO -DPROD1
INC_FILES = -include "../Global.h" -include "../Console.h" 

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wno-format
CFLAGS += -Wno-int-conversion
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-nullability-completeness
CFLAGS += -Wno-pointer-sign
CFLAGS += -Wno-array-bounds
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-incompatible-pointer-types-discards-qualifiers

COMP_OPTS  = $(CFLAGS) $(SYMBOLS) $(INC_FILES)
LINK_OPTS  = $(CFLAGS) $(SYMBOLS) $(INC_FILES) $(INC_DIRS) 
EXTRA_OBJS = $(OBJ_DIR)/Misc.o $(OBJ_DIR)/Crc16.o $(OBJ_DIR)/unity.o $(OBJ_DIR)/Blade.o $(OBJ_DIR)/Heartbeats.o

all: clean $(TARGETLIB) unit_tests

# components

COMPONENTS=irda_pic.c irda_ack.c irda_dummy.c pwm_functions.c 
COMPONENT_SRCS=$(addprefix $(SRC_DIR)/,$(COMPONENTS))
COMPONENT_OBJS=$(addprefix $(OBJ_DIR)/,$(COMPONENTS:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) ./obj
	gcc -c  -o $@ $< $(COMP_OPTS)

# extra objects

$(OBJ_DIR)/Misc.o: ../Misc.c
	$(MKDIR) ./obj
	gcc -c $(COMP_OPTS) ../Misc.c -o $(OBJ_DIR)/Misc.o

$(OBJ_DIR)/Crc16.o: ../Crc16.c
	gcc -c $(COMP_OPTS) ../Crc16.c -o $(OBJ_DIR)/Crc16.o

$(OBJ_DIR)/unity.o: $(UNITY_ROOT)/src/unity.c
	gcc -c $(CFLAGS) $(UNITY_ROOT)/src/unity.c -o $(OBJ_DIR)/unity.o

$(OBJ_DIR)/Blade.o: ../Blade.c
	gcc -c $(COMP_OPTS) ../Blade.c -o $(OBJ_DIR)/Blade.o

$(OBJ_DIR)/Heartbeats.o: ../Heartbeats.c
	gcc -c $(COMP_OPTS) ../Heartbeats.c -o $(OBJ_DIR)/Heartbeats.o

# lib.a and targets

TARGET1=$(OBJ_DIR)/test_irda_pic.out
TARGET2=$(OBJ_DIR)/test_irda_ack.out
TARGET3=$(OBJ_DIR)/test_irda_dummy.out
TARGET4=$(OBJ_DIR)/test_pwm_functions.out
TARGET5=$(OBJ_DIR)/test_base.out
TARGETS=$(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5)

$(TARGETLIB): $(COMPONENT_OBJS)
	ar rcs $(TARGETLIB) $(COMPONENT_OBJS)

$(TARGET1): $(TARGET_LIB) $(EXTRA_OBJS) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_irda_pic.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET1)

$(TARGET2): $(TARGET_LIB) $(EXTRA_OBJS) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_irda_ack.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET2)

$(TARGET3): $(TARGET_LIB) $(EXTRA_OBJS) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_irda_dummy.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET3)

$(TARGET4): $(TARGET_LIB) $(EXTRA_OBJS) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_pwm_functions.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET4)

$(TARGET5): $(TARGET_LIB) $(EXTRA_OBJS) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_base.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET5)


.PHONY: clean
clean:
	@echo "Clean up"
	$(CLEANUP) $(OBJ_DIR)/*
.PHONY: unit_tests
unit_tests: $(TARGETS) 
	@echo "Unit-tests (A, B, C, D)"
	@echo "-----------------------"
	$(TARGET1)
	$(TARGET2)
	$(TARGET3)
	$(TARGET4)
	$(TARGET5)
	
