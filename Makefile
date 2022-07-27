#
#

# Optimization flag
OPT_FLAG = -Os

#

CC      = /opt/amiga/bin/m68k-amigaos-gcc
STRIP   = /opt/amiga/bin/m68k-amigaos-strip
CFLAGS  = -Wpointer-arith -noixemul -flto -s -g0 $(OPT_FLAG) -m68020-60 -Wall -Wno-pointer-sign 
LDFLAGS = -lm -noixemul -flto

TARGET  = IT2C

BUILD_DIR ?= ./build
SRC_DIR ?= ./src
INC_DIR ?= ./include
OBJ_DIR ?= objdir
SRC = $(shell find $(SRC_DIR) -name *.c)
objects := $(SRC:%=$(BUILD_DIR)/%.o)

INC_FLAGS := $(addprefix -I,$(INC_DIR))

$(TARGET): $(objects)
	@$(CC) $(CFLAGS) $(objects) -o $@ $(LDFLAGS)
	@$(STRIP) $@ --strip-all
	@echo "-- Link finished --"

$(BUILD_DIR)/%.c.o : %.c
	@$(MKDIR_P) $(dir $@)
	$(CC) $(INC_FLAGS) $(CFLAGS) -c $< -o $@	

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

MKDIR_P ?= mkdir -p

