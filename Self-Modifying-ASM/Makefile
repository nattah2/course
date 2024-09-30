##
# Assembly Template
#
# @file
# @version 0.1

# Compiler and flags
AS = nasm
ASFLAGS = -f elf64

# Source and output files
SRC = bash.asm
OBJ = bash.o
OUT = bash

# Default target
all: $(OUT)

# Rule to assemble the source file
$(OBJ): $(SRC)
	$(AS) $(ASFLAGS) $< -o $@

# Rule to link object files
$(OUT): $(OBJ)
	ld $< -o $@

# Clean rule
clean:
	rm -f $(OBJ) $(OUT)

# end
