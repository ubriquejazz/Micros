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
CFLAGS += -Wno-implicit-function-declaration
CFLAGS += -Wno-incompatible-pointer-types-discards-qualifiers

SYMBOLS=-DMOCK -DPROD1
INC_FILES=-include "../enum.h" -include "../console.h" 
COMP_OPTS=$(CFLAGS) $(SYMBOLS) $(INC_FILES)

all: clean $(TARGETLIB) tests

# components

COMPONENTS=comp_A.c comp_B.c
COMPONENT_SRCS=$(addprefix $(SRC_DIR)/,$(COMPONENTS))
COMPONENT_OBJS=$(addprefix $(OBJ_DIR)/,$(COMPONENTS:.c=.o))

$(OBJ_DIR)/comp_A.o: $(SRC_DIR)/comp_A.c
	gcc -c $(COMP_OPTS) $(SRC_DIR)/comp_A.c -o $(OBJ_DIR)/comp_A.o

$(OBJ_DIR)/comp_B.o: $(SRC_DIR)/comp_B.c
	gcc -c $(COMP_OPTS) $(SRC_DIR)/comp_B.c -o $(OBJ_DIR)/comp_B.o

$(OBJ_DIR)/unity.o: $(UNITY_ROOT)/src/unity.c
	gcc -c $(CFLAGS) $(UNITY_ROOT)/src/unity.c -o $(OBJ_DIR)/unity.o

$(OBJ_DIR)/miscellaneous.o: ../miscellaneous.c
	gcc -c $(COMP_OPTS) ../miscellaneous.c -o $(OBJ_DIR)/miscellaneous.o

# lib.a and targets

TESTS = test_A test_B test_C test_D
TARGET1=$(OBJ_DIR)/test_A.out
TARGET2=$(OBJ_DIR)/test_B.out
LINK_OPTS=$(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(INC_FILES) 
EXTRA=$(OBJ_DIR)/unity.o $(OBJ_DIR)/miscellaneous.o

$(TARGETLIB): $(COMPONENT_OBJS)
	ar rcs $(TARGETLIB) $(COMPONENT_OBJS)

$(TARGET1): $(TARGET_LIB) $(EXTRA) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_A.c $(EXTRA) $(TARGETLIB) -o $(TARGET1)

$(TARGET2): $(TARGET_LIB) $(EXTRA) $($COMPONENT_OBJS)
	gcc $(LINK_OPTS) test_B.c $(EXTRA) $(TARGETLIB) -o $(TARGET2)


.PHONY: clean
clean:
	@echo "Clean up"
	$(CLEANUP) $(OBJ_DIR)/*

.PHONY: tests
tests: $(TARGET1) $(TARGET2)
	@echo "Unit-tests (A, B, C, D)"
	@echo "-----------------------"
	$(TARGET1)
	$(TARGET2)




