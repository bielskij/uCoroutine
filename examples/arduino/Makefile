export PROJECT_ROOT = $(shell pwd)

# Build verbosity
ifneq ($(V),1)
	MAKEFLAGS += --silent
endif

ifeq ($(CROSS_PREFIX),)
	CROSS_PREFIX := avr-
endif

CCACHE := ccache
ifeq (, $(shell which $(CCACHE)))
	CCACHE :=
endif

# Toolchain configuration
ifneq ($(V),1)
CC      := @$(CCACHE) $(CROSS_PREFIX)gcc
AS      := @$(CCACHE) $(CROSS_PREFIX)gcc -x assembler-with-cpp
OBJCOPY := @$(CCACHE) $(CROSS_PREFIX)objcopy
OBJDUMP := @$(CCACHE) $(CROSS_PREFIX)objdump
SIZE    := @$(CCACHE) $(CROSS_PREFIX)size
else
CC      := $(CCACHE) $(CROSS_PREFIX)gcc
AS      := $(CCACHE) $(CROSS_PREFIX)gcc -x assembler-with-cpp
OBJCOPY := $(CCACHE) $(CROSS_PREFIX)objcopy
OBJDUMP := $(CCACHE) $(CROSS_PREFIX)objdump
SIZE    := $(CCACHE) $(CROSS_PREFIX)size
endif

# common directories
DIR_OUT := out
DIR_INC := include
DIR_SRC := src
DIR_LIB := lib

# common CFLAGS
CFLAGS += -Wall -Wundef -Warray-bounds -Wformat -Wmissing-braces -Wreturn-type

# Compile flags
CFLAGS += -ffunction-sections -fdata-sections 
CFLAGS += -I$(DIR_INC) -I../../uCoroutine/include
CFLAGS += -mmcu=atmega328p -DF_CPU=16000000 -Os


LDFLAGS := -Wl,--gc-sections
LDFLAGS += -Wl,-Map,$(DIR_OUT)/$(APPLICATION_NAME).map,--cref
LDFLAGS += $(LDFLAGS_ORIG)

ifeq ($(DEBUG),1)
	CFLAGS += -DDEBUG=1 -ggdb
endif

SRC_APP := $(shell find $(DIR_SRC) -name '*.c' | sort)
TMP_APP := $(foreach file, $(SRC_APP), $(shell echo $(file) | sed -e 's|$(DIR_SRC)\/|$(DIR_OUT)\/|'))
OBJS_APP := $(foreach file, $(TMP_APP), $(shell echo $(file) | sed -e 's|\.c$$|.o|')) 

SRCS_UC := $(shell find ../../uCoroutine/src/ -name *.c)
OBJS_UC := $(patsubst ../../uCoroutine/src/%.c, $(DIR_OUT)/uc/%.o, $(SRCS_UC))

OBJ := $(OBJS_APP) $(OBJS_UC)

APPLICATION_NAME := example

init:
	mkdir -p $(DIR_OUT)

all: init $(OBJ) $(DIR_OUT)/$(APPLICATION_NAME).elf $(DIR_OUT)/$(APPLICATION_NAME).lss

bin: all $(DIR_OUT)/$(APPLICATION_NAME).hex
	$(SIZE) --format=avr $(DIR_OUT)/$(APPLICATION_NAME).elf

burn: bin
	avrdude -p m328p -P /dev/ttyUSB0 -c arduino -b57600 -U flash:w:$(DIR_OUT)/$(APPLICATION_NAME).hex:i

tests:
	make -C test/unit clean test

clean:
	rm -rf $(DIR_OUT)


$(DIR_OUT)/%.o: $(DIR_SRC)/%.c
	@echo "Building `basename $@`"
	mkdir -p `dirname $@`
	$(CC) -c $(CFLAGS) -o $@ $<

$(DIR_OUT)/uc/%.o: ../../uCoroutine/src/%.c
	@echo "Building `basename $@`"
	mkdir -p `dirname $@`
	$(CC) -c $(CFLAGS) -o $@ $<

$(DIR_OUT)/%.elf: $(OBJ)
	@echo "Building binary... $(APPLICATION_NAME).elf"
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)
	
$(DIR_OUT)/%.lss: $(DIR_OUT)/%.elf
	@echo "Creating Extended Listing... $@"
	$(OBJDUMP) -h -S $< > $@
	
$(DIR_OUT)/%.hex: $(DIR_OUT)/%.elf
	@echo "Create Flash image (ihex format)... $@"
	$(OBJCOPY) -R .eeprom -O ihex $< $@

$(DIR_OUT)/%.bin: $(DIR_OUT)/%.elf
	@echo "Create Flash image (binary format)... $@"
	$(OBJCOPY) $(OBJCOPY_BIN_FLAGS) -R .eeprom -O binary $< $@
	
$(DIR_OUT)/%.eep: $(DIR_OUT)/%.elf
	@echo "Create eeprom image (ihex format)... $@"
	$(OBJCOPY) -j .eeprom -O ihex $< $@