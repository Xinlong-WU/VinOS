BUILD_PATH = ./build

CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS32 = -nostdlib -fno-builtin -march=rv32ima -mabi=ilp32 -g -Wall -D RV32
CFLAGS64 = -nostdlib -fno-builtin -march=rv64ima -mabi=lp64 -g -Wall -mcmodel=medany -D RV64
CFLAGS := ${CFLAGS64}

QEMU32 = qemu-system-riscv32
QEMU64 = qemu-system-riscv64
QEMU := ${QEMU64}

ifeq (${arch}, rv32)
CFLAGS := ${CFLAGS32}
QEMU := ${QEMU32}
else
arch := rv64
endif
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump

SRCS_ASM = \
	start.S \
	memInfo.S \
	entry.S \

SRCS_C = \
	kernel.c \
	uart.c \
	utils.c \
	memory.c \
	sched.c	\
	user.c	\
	trap.c	\
	plic.c	\
	timer.c \

OBJS = $(SRCS_ASM:.S=.o)
OBJS += $(SRCS_C:.c=.o)

.DEFAULT_GOAL := all
all: os.elf
	@echo "Build Arch: ${arch}"

# start.o must be the first in dependency!
os.elf: ${OBJS}
	${CC} $(CFLAGS) -T os.ld -o os.elf $^
	${OBJCOPY} -O binary os.elf os.bin

%.o : %.c
	${CC} ${CFLAGS} -c -o $@ $<

%.o : %.S
	${CC} ${CFLAGS} -c -o $@ $<

run: all
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf

.PHONY : debug
debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf -s -S &
	@${GDB} os.elf -q -x gdbinit

.PHONY : code
code: all
	@${OBJDUMP} -S os.elf | less

.PHONY : clean
clean:
	rm -rf ${BUILD_PATH}
	rm -f *.elf
	rm -f *.o
	rm -f *.bin

qemu:
	@${QEMU} -M ? | grep virt >/dev/null || exit
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel os.elf
