# We try to detect the OS
RM = rm -f
MKDIR = mkdir -p

SRC_DIR=./src
OBJ_DIR=./obj
PROJECT_ROOT=../../..

TARGETLIB = $(OBJ_DIR)/libtesters.a
UNITY_ROOT= $(PROJECT_ROOT)/Unity
INC_DIRS  = -I$(UNITY_ROOT)/src
SYMBOLS   = 
INC_FILES = -include $(SRC_DIR)/list.h

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wno-format
CFLAGS += -Wno-int-conversion
CFLAGS += -Wno-unused-variable
CFLAGS += -Wno-nullability-completeness
CFLAGS += -Wno-pointer-sign
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-incompatible-pointer-types-discards-qualifiers

COMP_FLAGS  = $(CFLAGS) $(SYMBOLS)
LINK_FLAGS  = $(CFLAGS) $(SYMBOLS) $(INC_DIRS) $(INC_FILES)
EXTRA_OBJS = $(OBJ_DIR)/unity.o

all: clean $(TARGETLIB) tests

# components

COMPONENTS=main.c list.c
COMPONENT_SRCS=$(addprefix $(SRC_DIR)/,$(COMPONENTS))
COMPONENT_OBJS=$(addprefix $(OBJ_DIR)/,$(COMPONENTS:.c=.o))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) ./obj
	gcc -c  -o $@ $< $(COMP_FLAGS)

# extra objects 

$(OBJ_DIR)/unity.o: $(UNITY_ROOT)/src/unity.c
	gcc -c $(CFLAGS) $(UNITY_ROOT)/src/unity.c -o $(OBJ_DIR)/unity.o

# lib.a and targets

TARGET1=$(OBJ_DIR)/test_A.out
TARGET2=$(OBJ_DIR)/test_B.out
TARGET3=$(OBJ_DIR)/main.a

$(TARGETLIB): $(COMPONENT_OBJS)
	ar rcs $(TARGETLIB) $(COMPONENT_OBJS)

$(TARGET1): $(TARGET_LIB) $(EXTRA_OBJS) $(COMPONENT_OBJS)
	gcc $(LINK_FLAGS) test_A.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET1)

$(TARGET2): $(TARGET_LIB) $(EXTRA_OBJS) $(COMPONENT_OBJS)
	gcc $(LINK_FLAGS) test_B.c $(EXTRA_OBJS) $(TARGETLIB) -o $(TARGET2)

$(TARGET3): $(TARGET_LIB) $(COMPONENT_OBJS)
	gcc $(LINK_FLAGS) $(SRC_DIR)/main.c $(TARGETLIB) -o $(TARGET3)


.PHONY: clean
clean:
	@echo "Clean up"
	$(RM) $(OBJ_DIR)/*

.PHONY: tests
tests: $(TARGET1) $(TARGET2) $(TARGET3)
	@echo "Unit-tests (A, B, C, D)"
	@echo "-----------------------"
	$(TARGET1)
	$(TARGET2)
	$(TARGET3)
