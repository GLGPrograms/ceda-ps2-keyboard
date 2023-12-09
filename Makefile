# Detect repository root directory
REPODIR ?= $(realpath $(CURDIR))
QUIET := @

# Name of the project
PROJ_NAME   = ceda-keyboard

## Directories ##
# This is where the source files are located,
# which are not in the current directory
SRC_DIR       = $(REPODIR)/src

# The header files we use are located here
INC_DIR   =  $(REPODIR)/include

BUILD_DIR     = $(REPODIR)/build
OUTPUT_DIR    = $(REPODIR)/output

######################################################################
#                             SOURCES                                #
######################################################################

## FILES ##
# c files
# SRCS        =
SRCS		+= \
			   src/main.c \
			   src/timer.c \
			   \

# asm files
ASRC 		+=

# Object files
# Automatically declares object file names
OBJS          = $(patsubst %.c,   $(BUILD_DIR)/%.c.o,  $(filter %.c,$(SRCS)) )
OBJS         += $(patsubst %.cpp, $(BUILD_DIR)/%.cpp.o,$(filter %.cpp,$(SRCS)) )

# Dependencies from .h/.hpp files, but exclude asm files
DEPS         := $(patsubst %.o, %.d, $(OBJS))

OBJS         += $(patsubst %.s,   $(BUILD_DIR)/%.s.o,  $(filter %.s,$(ASRC)) )
OBJS         += $(patsubst %.S,   $(BUILD_DIR)/%.S.o,  $(filter %.S,$(ASRC)) )

######################################################################
#                         SETUP TOOLS                                #
######################################################################
ECHO      := /usr/bin/env echo

# Toolchain
CC         = avr-gcc
CXX        = avr-g++
OBJCOPY    = avr-objcopy
OBJDUMP    = avr-objdump
GDB        = avr-gdb
AS         = avr-as
SIZE       = avr-size
AVRDUDE	   = avrdude
HOSTCC     = gcc
HOSTCXX    = g++

# Microcontroller
MCU        = atmega328p
CPU_FREQ   = 8000000
# Fuses -- www.engbedded.com/fusecalc/
LFUSE			 = 0xe2
HFUSE			 = 0xd9

### GCC options ###

## Compiler flags ##
# Do not run the linker
CFLAGS    += -c
# Auto optimisation
CFLAGS    += -Os
# All warning messages
CFLAGS    += -Wall -Wextra
# Puts functions and data into its own section - remove thread-safe things
CFLAGS    += -fno-exceptions -fstack-usage -fdump-tree-optimized -ffunction-sections -fdata-sections
# Use smallest size for enums
CFLAGS    += -fshort-enums
# Microcontroller
CFLAGS    += -mmcu=$(MCU)
# Clock speed
CFLAGS    += -DCPU_FREQ=$(CPU_FREQ)L
# Header files
CFLAGS    += $(addprefix -I,$(INC_DIR))
CFLAGS    += -flto

# Linker flags
LFLAGS     = -mmcu=$(MCU)
LFLAGS    += $(addprefix -I,$(INC_DIR))

######################################################################
#                      PROGRAMMING TOOLS                             #
######################################################################
# To match MCU with BOARD, see link
# http://www.nongnu.org/avr-libc/user-manual/using_tools.html
PROGRAMMER = usbasp
# verbose
# PROGRAM_FLAGS =  -v
# erase
# PROGRAM_FLAGS =  -e
# choose programmer
PROGRAM_FLAGS += -c $(PROGRAMMER)
# target cpu
PROGRAM_FLAGS += -p $(MCU)
# USB port and baudrate
# PROGRAM_FLAGS += -P /dev/ttyACM0

######################################################################
#                             TARGETS                                #
######################################################################
.PHONY: clean

all:     $(OUTPUT_DIR)/$(PROJ_NAME).hex

-include $(DEPS)

# invoke CC compiler before assemblying
$(BUILD_DIR)/%.S.o : %.S
	@echo -e "\033[1;33m[AS      ]\033[0m " $(notdir $<)
	@mkdir -p `dirname $@`
	$(QUIET) $(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.s.o : %.s
	@echo -e "\033[1;33m[AS      ]\033[0m " $(notdir $<)
	@mkdir -p `dirname $@`
	$(QUIET) $(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.c.o:  %.c
	@echo -e "\033[1;33m[CC      ]\033[0m " $(notdir $<)
	@mkdir -p `dirname $@`
	$(QUIET) $(CC) $(CFLAGS) $< -o $@

.DELETE_ON_ERROR:
$(BUILD_DIR)/%.c.d: %.c
	@mkdir -p `dirname $@`
	@$(ECHO) -n "$@ " > $@
	@$(CC) -MM -MT '$(@D)/$(<F).o' $(CFLAGS) $< >> $@

.DELETE_ON_ERROR:
$(BUILD_DIR)/%.cpp.d: %.cpp
	@mkdir -p `dirname $@`
	@$(ECHO) -n "$@ " > $@
	@$(CC) -MM -MT '$(@D)/$(<F).o' $(CFLAGS) $< >> $@

$(OUTPUT_DIR)/$(PROJ_NAME).elf: $(OBJS)
	@echo -e "\033[1;34m[LD      ]\033[0m " $(notdir $@)
	@mkdir -p ${OUTPUT_DIR}
	$(QUIET) $(CC) $(LFLAGS) -o $@ $(foreach file, $^, $(file)) -lm
	@echo -e "\033[1;35m[OBJDUMP ]\033[0m " $(notdir $^)
	$(QUIET) $(OBJDUMP) -h -S $@ > $(OUTPUT_DIR)/$(PROJ_NAME).lss

$(OUTPUT_DIR)/$(PROJ_NAME).hex: $(OUTPUT_DIR)/$(PROJ_NAME).elf
	@echo -e "\033[1;36m[HEX     ]\033[0m " $(notdir $^)
	$(QUIET) $(OBJCOPY) -O ihex -R .eeprom $^ $@

size: $(OUTPUT_DIR)/$(PROJ_NAME).elf
	$(SIZE) -C --mcu=$(MCU) $(OUTPUT_DIR)/$(PROJ_NAME).elf

flash:		$(OUTPUT_DIR)/$(PROJ_NAME).hex
	$(AVRDUDE) $(PROGRAM_FLAGS) -U flash:w:$(OUTPUT_DIR)/$(PROJ_NAME).hex

fuse:			$(OUTPUT_DIR)/$(PROJ_NAME).hex
	$(AVRDUDE) $(PROGRAM_FLAGS) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

test:
	$(AVRDUDE) $(PROGRAM_FLAGS)

clean:
	@echo -e "\033[1;33m[CLEAN	]\033[0m"
	@rm -rf $(BUILD_DIR)/*
	@rm -rf $(OUTPUT_DIR)/*

