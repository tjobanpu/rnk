TARGET_NAME := queue_test.elf

OBJS := queue_test.o

MCPU=cortex-m4

INCLUDES	+= -I$(KERNEL_BASE)/include
INCLUDES	+= -I$(KERNEL_BASE)/boards
INCLUDES	+= -I$(KERNEL_BASE)/lib
INCLUDES	+= -include $(KERNEL_BASE)/config.h

CFLAGS  :=  -Wall -mlong-calls -fno-builtin -ffunction-sections
CFLAGS += -mcpu=$(MCPU) -mthumb -nostdlib $(INCLUDES)
CFLAGS +=  -mword-relocations -fno-common

LDFLAGS	:= -g $(INCLUDES) -nostartfiles -nostdlib #-Wl,--gc-sections
LDFLAGS += -r -Bsymbolic -mcpu=$(MCPU) -mthumb -mlong-calls -fno-common

LD_SCRIPT = $(APPS_BASE)/apps.ld

CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
AR := $(CROSS_COMPILE)ar
LD := $(CROSS_COMPILE)ld

all: $(TARGET_NAME)

clean:
	$(RM) $(TARGET_NAME)
	$(RM) $(OBJS)
	
$(TARGET_NAME): $(OBJS) 
	@@echo "LD " $@
	@$(CC) $(LDFLAGS) -T$(LD_SCRIPT) -o $@ $^

%.o: %.c
	@@echo "CC " $<
	@$(CC) $(CFLAGS) -c $< -o $@
