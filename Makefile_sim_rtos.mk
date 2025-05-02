PROJECT = simulated-rtos
BUILD_DIR = bin

CFILES := sim_rtos.c $(wildcard lib/*.c)
INCLUDES += -Ilib

DEVICE=stm32f407vgt6
OOCD_FILE = board/stm32f4discovery.cfg

OPENCM3_DIR=libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

