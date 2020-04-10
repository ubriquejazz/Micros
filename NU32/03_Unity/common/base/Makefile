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
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-incompatible-pointer-types-discards-qualifiers

COMP_OPTS  = $(CFLAGS) $(SYMBOLS) $(INC_FILES)
LINK_OPTS  = $(CFLAGS) $(SYMBOLS) $(INC_FILES) $(INC_DIRS) 
EXTRA_OBJS = $(OBJ_DIR)/Misc.o $(OBJ_DIR)/Crc16.o $(OBJ_DIR)/unity.o

all: clean $(TARGETLIB) unit_tests

# components

# COMPONENTS=comp_A.c comp_B.c
# COMPONENT_SRCS=$(addprefix $(SRC_DIR)/,$(COMPONENTS))
# COMPONENT_OBJS=$(addprefix $(OBJ_DIR)/,$(COMPONENTS:.c=.o))
COMPONENT_OBJS=$(EXTRA_OBJS)

# $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
# 	$(MKDIR) ./obj
# 	gcc -c  -o $@ $< $(COMP_OPTS)

# extra objects

$(OBJ_DIR)/Misc.o: ../Misc.c
	$(MKDIR) ./obj
	gcc -c $(COMP_OPTS) ../Misc.c -o $(OBJ_DIR)/Misc.o

$(OBJ_DIR)/Crc16.o: ../Crc16.c
	gcc -c $(COMP_OPTS) ../Crc16.c -o $(OBJ_DIR)/Crc16.o

$(OBJ_DIR)/unity.o: $(UNITY_ROOT)/src/unity.c
	gcc -c $(CFLAGS) $(UNITY_ROOT)/src/unity.c -o $(OBJ_DIR)/unity.o

# lib.a and targets

TARGET1=$(OBJ_DIR)/test_A.out
TARGET2=$(OBJ_DIR)/test_B.out

$(TARGETLIB): $(COMPONENT_OBJS)
	ar rcs $(TARGETLIB) $(COMPONENT_OBJS)

$(TARGET1): $(TARGET_LIB) $(EXTRA_OBJS) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_A.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET1)

$(TARGET2): $(TARGET_LIB) $(EXTRA_OBJS) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_B.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET2)

.PHONY: clean
clean:
	@echo "Clean up"
	$(CLEANUP) $(OBJ_DIR)/*
	
.PHONY: unit_tests
unit_tests: $(TARGET1) $(TARGET2)
	@echo "Unit-tests (A, B, C, D)"
	@echo "-----------------------"
	$(TARGET1)
	$(TARGET2)




