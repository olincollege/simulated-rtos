PROJECT = simulated-rtos
BUILD_DIR = bin

CFILES := sim_rtos.c $(wildcard lib/*.c)#changed
INCLUDES += -Ilib

OBJS := $(CFILES:.c=.o)#added
DEPS := $(OBJS:.o=.d)#added


CFLAGS += -Wall -Wextra -Os -Ilib -MD -MP#added

DEVICE=stm32f407vgt6
OOCD_FILE = board/stm32f4discovery.cfg

OPENCM3_DIR=libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
#added below
.PHONY: clean
clean:
	rm -f $(OBJS) $(DEPS) $(PROJECT).elf $(PROJECT).bin $(PROJECT).hex

