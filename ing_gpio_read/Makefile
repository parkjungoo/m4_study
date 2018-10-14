# All the files will be generated with this name (main.elf, main.bin, main.hex, etc)
PROJ_NAME = main

# GNU ARM Embedded Toolchain
CC = arm-none-eabi-gcc
CXX = arm-none-eabi-g++
LD = arm-none-eabi-ld
AR = arm-none-eabi-ar
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
OD = arm-none-eabi-objdump
NM = arm-none-eabi-nm
SIZE = arm-none-eabi-size

# Directory Structure
BINDIR = bin
LINKSCRIPTPATH = linkscript

# Find source files
ASOURCES = $(shell find -L . -name '*.s')
CSOURCES += $(shell find -L . -name '*.c')

# Find header directories
INC = $(shell find -L . -name '*.h' -exec dirname {} \; | uniq)
INCLUDES = $(INC:%=-I%)

# Create object list
OBJECTS = $(ASOURCES:%.s=%.o)
OBJECTS += $(CSOURCES:%.c=%.o)

DEPSOBJ = $(CSOURCES:%.c=%.d)

# Define output files ELF & IHEX
OUT_ELF = $(PROJ_NAME).elf
OUT_HEX = $(PROJ_NAME).hex
OUT_BIN = $(PROJ_NAME).bin

# MCU FLAGS
MCFLAGS = -mcpu=cortex-m4 -mthumb -mlittle-endian \
		  -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork
# COMPILE FLAGS
DEFS = -DUES_STDPERIPH_DRIVER -DSTM32F4XX
CFLAGS += -c $(MCFLAGS) $(DEFS) $(INCLUDES)

# LINKER FLAGS
LDSCRIPT = $(LINKSCRIPTPATH)/stm32_flash.ld
LDFLAGS = -T $(LDSCRIPT) $(MCFLAGS) --specs=nosys.specs

# Build Rules
.PHONY: all release release-memopt debug clean

###################################################

all: release-memopt

release: $(BINDIR)/$(OUT_HEX)

release-memopt: CFLAGS += -Os
release-memopt: LDFLAGS += -Os
release-memopt: release

debug: CFLAGS += -g
debug: LDFLAGS += -g
debug: release

$(BINDIR)/$(OUT_HEX): $(BINDIR)/$(OUT_ELF)
	$(OBJCOPY) -O ihex $< $@
	$(OBJCOPY) -O binary $< $(BINDIR)/$(OUT_BIN)
	@echo "Objcopy from ELF to binary complete!\n"

$(BINDIR)/$(OUT_ELF): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@
	@echo "Linking complete!\n"
	$(SIZE) $(BINDIR)/$(OUT_ELF)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	$(CC) -MM $(CFLAGS) $< > $*.d
	@echo "Compiled "$<"!\n"

%.o: %.s
	$(CC) $(CFLAGS) $< -o $@
	@echo "Assembled "$<"!\n"

flash:
	st-flash write $(BINDIR)/$(OUT_BIN) 0x8000000

clean:
	rm -f $(OBJECTS) $(DEPSOBJ) 
	rm -f $(BINDIR)/$(OUT_ELF)
	rm -f $(BINDIR)/$(OUT_HEX)
	rm -f $(BINDIR)/$(OUT_BIN)
