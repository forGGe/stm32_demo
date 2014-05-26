TARGET		:= demo
BUILDDIR	:= ./build

ELF		:= $(BUILDDIR)/$(TARGET).elf
BIN		:= $(BUILDDIR)/$(TARGET).bin

SRCS		:= $(wildcard *.c)
OBJS		:= $(addprefix $(BUILDDIR)/, $(SRCS:.c=.o))

CC		:= arm-none-eabi-gcc
LD		:= arm-none-eabi-ld
OBJCPY		:= arm-none-eabi-objcopy

# debugging symbols will be stripped
# of the elf, so it won't affect
# binary size
CCFLAGS		:= -c -fno-common -Os -g
# platform flags
CCFLAGS		+= -mcpu=cortex-m4 -mthumb
# reorder data and function sections so
# unused symbols could be stripped
CCFLAGS		+= -fdata-sections -ffunction-sections
# remove unused symbols
LDFLAGS		:= -nostartfiles --gc-sections
OBJFLAGS	:= -Obinary

INCLUDES	:= -I.
LDSCRIPTS	:= -Tstm32.ld

# STM library stuff

LIB	 := stm32_lib
LIBPATH	 := ./lib
LIBSRCS	 := $(wildcard $(LIBPATH)/src/*.c)
LIBINCL	 := $(LIBPATH)/inc
LIBOBJS	 := $(addprefix $(BUILDDIR)/, $(notdir $(LIBSRCS:.c=.o)))

INCLUDES	+= -I$(LIBINCL)
OBJS		+= $(LIBOBJS)


all: $(TARGET)

clean:
	rm -vf $(ELF) $(OBJS) $(BIN)

$(TARGET):
	@echo "making binary"
	mkdir -p $(BUILDDIR)
	make $(BIN)
	@echo
	@echo "				---- {{{ BUILD SUCCESSFULL }}} ----"
	@echo

$(BIN): $(ELF)
	@echo "stripping into the binary"
	$(OBJCPY) $(OBJFLAGS) $< $@

$(ELF): $(OBJS)
	@echo "generating ELF"
	$(LD) $(LDSCRIPTS) $(LDFLAGS) -o $@ $^

# this is for main project sources
$(BUILDDIR)/%.o: ./%.c
	$(CC) $(INCLUDES) $(CCFLAGS) $< -o $@

# this is for stm32 periph library sources
$(BUILDDIR)/%.o: $(LIBPATH)/src/%.c
	$(CC) $(INCLUDES) $(CCFLAGS) $< -o $@


#arm-none-eabi-gcc -I. -c -fno-common -O0 -g -mcpu=cortex-m3 -mthumb main.c
#arm-none-eabi-ld -Tstm32.ld -nostartfiles -o main.elf main.o
#arm-none-eabi-objcopy -Obinary main.elf main.bin