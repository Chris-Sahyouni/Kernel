# ======== VARIABLES ========
# Automatic variables
#  $@ the target name
#  $? all prerequisites that are newer than the target
#  $^ all prerequisites
#  $< the first prequisite
# Wildcards
# * searches the filesystem for a matching name
# % one of two things:
#   - matches a set of characters in a string
#   - replaces matched characters in a string

ASSEMBLER := nasm
CC := gcc
LINKER := ld
CFLAGS := -Wall -O -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -m32 -ffreestanding -fno-pic

BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)/bin
SRC_DIR := src
INCLUDE_DIR := include

SRC_FILES := $(wildcard src/*.c)
OBJ_FILES := $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# ======== TARGETS ========
# <targets>: <prerequisites> : tells make that the dependencies must exist before targets can
#	 <command>
#	 <command>
# 	 ...


.PHONY: all
all: $(BIN_DIR)/kernel.bin # the first listed rule runs by default when none is specified


# Compile sources to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCLUDE_DIR)/system.h
	$(CC) $(CFLAGS) -I./$(INCLUDE_DIR) -c -o $@ $<

# Compile start assembly file
$(BUILD_DIR)/start.o: start.asm
	echo Assembling start.asm
	$(ASSEMBLER) -f elf32 -o $@ start.asm


# LINK to final executable
$(BIN_DIR)/kernel.bin: link.ld $(BUILD_DIR)/start.o $(OBJ_FILES)
	echo Linking object files
	$(LINKER) -T link.ld -m elf_i386 -o $@ $(OBJ_FILES) $(BUILD_DIR)/start.o


# PHONY tells make this target is not a file
.PHONY: clean
clean:
	find build -type f -delete
