# We try to detect the OS
CLEANUP = rm -f
MKDIR = mkdir -p

SRC_DIR=./src
OBJ_DIR=./obj
PROJECT_ROOT=../..

TARGETLIB=$(OBJ_DIR)/libtesters.a
UNITY_ROOT=$(PROJECT_ROOT)/external/libs/Unity
INC_DIRS =-I$(UNITY_ROOT)/src

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

SYMBOLS=-DMOCK -DVERBO -DPROD1
INC_FILES=-include "../enum.h" -include "../console.h" 
COMP_OPTS=$(CFLAGS) $(SYMBOLS) $(INC_FILES)

all: clean $(TARGETLIB) unit_tests

# components

COMPONENTS=blade.c heartbeats.c irda_pic.c irda_ack.c irda_dummy.c pwm_functions.c
COMPONENT_SRCS=$(addprefix $(SRC_DIR)/,$(COMPONENTS))
COMPONENT_OBJS=$(addprefix $(OBJ_DIR)/,$(COMPONENTS:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) ./obj
	gcc -c  -o $@ $< $(COMP_OPTS)

$(OBJ_DIR)/unity.o: $(UNITY_ROOT)/src/unity.c
	gcc -c $(CFLAGS) $(UNITY_ROOT)/src/unity.c -o $(OBJ_DIR)/unity.o

$(OBJ_DIR)/miscellaneous.o: ../miscellaneous.c
	gcc -c $(COMP_OPTS) ../miscellaneous.c -o $(OBJ_DIR)/miscellaneous.o

# lib.a and targets

TARGET1=$(OBJ_DIR)/test_irda_pic.out
TARGET2=$(OBJ_DIR)/test_irda_ack.out
TARGET3=$(OBJ_DIR)/test_irda_dummy.out
TARGET4=$(OBJ_DIR)/test_blade.out
TARGET5=$(OBJ_DIR)/test_heartbeats.out
TARGET6=$(OBJ_DIR)/test_pwm_functions.out
TARGETS=$(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4) $(TARGET5) $(TARGET6) 
LINK_OPTS=$(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(INC_FILES) 
EXTRA=$(OBJ_DIR)/unity.o $(OBJ_DIR)/miscellaneous.o

$(TARGETLIB): $(COMPONENT_OBJS)
	ar rcs $(TARGETLIB) $(COMPONENT_OBJS)

$(TARGET1): $(TARGET_LIB) $(EXTRA) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_irda_pic.c $(EXTRA) $(TARGETLIB) -o $(TARGET1)

$(TARGET2): $(TARGET_LIB) $(EXTRA) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_irda_ack.c $(EXTRA) $(TARGETLIB) -o $(TARGET2)

$(TARGET3): $(TARGET_LIB) $(EXTRA) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_irda_dummy.c $(EXTRA) $(TARGETLIB) -o $(TARGET3)

$(TARGET4): $(TARGET_LIB) $(EXTRA) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_blade.c $(EXTRA) $(TARGETLIB) -o $(TARGET4)

$(TARGET5): $(TARGET_LIB) $(EXTRA) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_heartbeats.c $(EXTRA) $(TARGETLIB) -o $(TARGET5)

$(TARGET6): $(TARGET_LIB) $(EXTRA) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_pwm_functions.c $(EXTRA) $(TARGETLIB) -o $(TARGET6)


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
	$(TARGET6)