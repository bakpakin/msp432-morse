#===============================================================
# Makefile for building MSP Code Examples in command line
# environement using the GCC Open Source Compiler for MSP432
# Modified by Calvin Rose
#===============================================================

DEVICE ?= MSP432P401R

# You should only need to change stuff here for the most part.

SOURCES         := main.c
TARGET          := morse.out
OBJ_DIR			:= output

######################################

# GCC Configuration
GCC_MSP_INC_DIR 	?= platform/include
GCC_CMSIS_INC_DIR 	?= $(GCC_MSP_INC_DIR)/CMSIS
LDDIR				:= $(GCC_MSP_INC_DIR)/$(shell echo $(DEVICE) | tr A-Z a-z)
RM					:= rm -rf
MKDIR				= mkdir -p -- $@
GCC_INC_DIR 	?= /usr/lib/arm-none-eabi/include
CC              := arm-none-eabi-gcc
GDB			    := arm-none-eabi-gdb
INCLUDES		:= -I $(GCC_CMSIS_INC_DIR) -I $(GCC_MSP_INC_DIR) -I $(GCC_INC_DIR)
COMMONFLAGS     := -mcpu=cortex-m4 -march=armv7e-m -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mthumb -D__$(DEVICE)__ -DTARGET_IS_MSP432P4XX -Dgcc -g -gstrict-dwarf -Wall
CFLAGS			:= $(COMMONFLAGS) -ffunction-sections -fdata-sections -MD -std=c99
LDFLAGS			:= $(COMMONFLAGS) -T$(LDDIR).lds -l'c' -l'gcc' -l'nosys'

ifeq ($(DEVICE), MSP432P401R)
SOURCES			+= startup_msp432p401r_gcc.c
SOURCES			+= system_msp432p401r.c
SOURCES         += interrupts_msp432p401r_gcc.c
else
SOURCES			+= startup_msp432p401m_gcc.c
SOURCES         += system_msp432p401m.c
SOURCES         += interrupts_msp432p401m_gcc.c
endif

# Use vpath to find source files
VPATH=src:platform/src

# Get the destination object files for all source files.
OBJECTS := $(addsuffix .o,$(addprefix $(OBJ_DIR)/, $(basename $(SOURCES))))

######################################

all: $(OBJ_DIR)/$(TARGET)

$(OBJECTS): | $(OBJ_DIR)

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@echo ============================================
	@echo Generating $@
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/$(TARGET): $(OBJECTS)
	@echo ============================================
	@echo Linking objects and generating output binary
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(INCLUDES)

debug: all
	$(GDB) $(OBJ_DIR)/$(TARGET)

clean:
	@$(RM) $(OBJ_DIR)
