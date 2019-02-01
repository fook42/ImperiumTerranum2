#
#

CC      = /opt/amiga/bin/m68k-amigaos-gcc
STRIP   = /opt/amiga/bin/m68k-amigaos-strip
#CFLAGS  = -noixemul -O0 -m68020 -Wall -Wno-pointer-sign 
CFLAGS  = -noixemul -flto -s -g0 -O2 -m68020-60 -Wall -Wno-pointer-sign 
TARGET  = IT2C
LDFLAGS = -lm -noixemul -flto   

OBJDIR = objdir
SRC = $(wildcard *.c)
objects := $(patsubst %.c,%.o,$(wildcard *.c))
# OBJS = $(addprefix $(OBJDIR)/,$(dir $(SOURCE)), $(notdir $(SOURCE:.c=.o)))

clean:
	@rm -f $(TARGET) || true
	@rm -f $(objects) || true

all : clean $(objects)
	@$(CC) $(CFLAGS) -o $(TARGET) $(objects) $(LDFLAGS)
	@rm -f $(objects)

$(TARGET): $(SRC)
	echo "============="
	echo "building the target $@"
	echo "============="
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@$(STRIP) $@ --strip-all
	@echo -- Link finished --

%.o : %.c
	$(CC) $(CFLAGS) -c $<
