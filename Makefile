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
COMPILER := gcc
LINKER := ld


# ======== TARGETS ========
# <targets>: <prerequisites> : tells make that the dependencies must exist before targets can
#	 <command>
#	 <command>
# 	 ...


.PHONY: all
all: kernel.bin # the first listed rule runs by default when none is specified

start.o: start.asm
	echo Assembling start.asm
	$(ASSEMBLER) -f elf32 -o $@ start.asm

kernel.bin: start.o link.ld
	echo Linking object files
	$(LINKER) -T link.ld -m elf_i386 -o $@ start.o

# PHONY tells make this target is not a file
.PHONY: clean
clean:
	rm -f kernel.bin start.o

