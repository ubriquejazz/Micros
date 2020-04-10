# We try to detect the OS
CLEANUP = rm -f
MKDIR = mkdir -p

SRC_DIR=./src
OBJ_DIR=./obj
PROJECT_ROOT=../..

TARGETLIB = libdevicewrapper.a
UNITY_ROOT=$(PROJECT_ROOT)/external/libs/Unity-master
INC_DIRS=-Iinc -I$(UNITY_ROOT)/src -I/usr/include/python3.5

CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Wpointer-arith
CFLAGS += -Wcast-align
CFLAGS += -Wwrite-strings
CFLAGS += -Wswitch-default
CFLAGS += -Wunreachable-code
CFLAGS += -Winit-self
CFLAGS += -Wmissing-field-initializers
CFLAGS += -Wno-unknown-pragmas
CFLAGS += -Wstrict-prototypes
CFLAGS += -ggdb
CFLAGS += -O0
CFLAGS += -Wundef
# TODO: don't forget to remove this later
CFLAGS += -Wno-unused-parameter
CFLAGS += -DLINUX_BUILD
CFLAGS += -D__DYNAMIC_MEMORY
CFLAGS += -D_POSIX_C_SOURCE=199309L
#define this if you want lots of output
ifdef DEBUG_PRINT
   CFLAGS += -DDPRINTF_ENABLED
endif
CFLAGS += -fPIC

TESTS = DeviceWrapper
TARGET1 = $(OBJ_DIR)/DeviceWrapper.so
TARGET2 =
TARGETS = $(TARGET1) $(TARGET2) 

all: clean  $(TARGETLIB) $(TARGETS) unit_tests

# components

COMPONENTS=DeviceWrapper.c
COMPONENT_SRCS=$(addprefix $(SRC_DIR)/,$(COMPONENTS))
COMPONENT_OBJS=$(addprefix $(OBJ_DIR)/,$(COMPONENTS:.c=.o))

SYMBOLS=
INC_FILES=
COMP_OPTS=$(CFLAGS) $(INC_DIRS) $(SYMBOLS) $(INC_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR) ./obj
	gcc -c  -o $@ $< $(COMP_OPTS)

$(OBJ_DIR)/unity.o: $(UNITY_ROOT)/src/unity.c
	gcc -c $(CFLAGS) $(UNITY_ROOT)/src/unity.c -o $(OBJ_DIR)/unity.o

# lib.a and targets

LINK_OPTS = $(CFLAGS) -pthread -shared

$(TARGETLIB): $(COMPONENT_OBJS)
	ar rcs $(TARGETLIB) $(COMPONENT_OBJS)

$(TARGET1): $(TARGET_LIB) $(OBJ_DIR)/unity.o $(COMPONENT_OBJS)
	gcc $(LINK_OPTS) -Wl,-soname=DeviceWrapper -o $(TARGET1) $(COMPONENT_OBJS)
	cp $(TARGET1) ./test

.PHONY: clean
clean:
	@echo "Clean up"
	$(CLEANUP) $(OBJ_DIR)/*.* *.out
	$(CLEANUP) ./test/*.so

.PHONY: unit_tests
unit_tests: $(TARGETS)
	@echo "About to run unit tests"
	/usr/bin/python3 -m pytest ./test/
#	$(foreach RUNTARGET, $(TARGETS), $(call execute-command,./$(RUNTARGET)))

