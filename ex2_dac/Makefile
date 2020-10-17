# Example Makefile
#
# Exercise 2, TDT4258

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

INC=inc
SRC=src
LIB=lib
BIN=bin

INCLUDEPATHS=-I${INC}

# Add -DINTERRUPT_DISABLED to disable interrupts
# Example: make ADDFLAGS=-DINTERRUPT_DISABLED
ADDFLAGS=
CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=c99 -Wall ${ADDFLAGS} ${INCLUDEPATHS}
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lc -lcs3 -lcs3unhosted -lefm32gg -Llib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
LINKERSCRIPT=lib/efm32gg.ld

bin/ex2.bin : bin/ex2.elf
	${OBJCOPY} -O binary $< $@

bin/ex2.elf : bin/ex2.o bin/timer.o bin/dac.o bin/gpio.o bin/interrupt_handlers.o bin/utils.o bin/music.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

bin/%.o : src/%.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY : pretty
pretty :
	-indent *.c *.h

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "${BIN}/ex2.bin" -r

.PHONY : clean
clean :
	-rm -rf bin/*.o bin/*.elf bin/*.bin bin/*.hex 
