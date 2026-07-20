# =============================================================================
# Makefile — Question 2: Process Monitor
# Description: Build automation for process_monitor.c
# Usage      : make         → Build the executable
#              make clean   → Remove build artifacts
#              make run     → Build and run
#              make check   → Check for memory issues with valgrind
# =============================================================================

# ---- Compiler Settings ----
CC      = gcc                          # Use GCC as the C compiler
CFLAGS  = -Wall -Wextra -Wpedantic    # Enable all warnings + pedantic checks
CFLAGS += -std=c11                     # Use C11 standard
CFLAGS += -D_POSIX_C_SOURCE=200809L   # Enable POSIX.1-2008 API
CFLAGS += -g                           # Include debug symbols (for gdb/valgrind)

# ---- Target Executable ----
TARGET  = process_monitor
SRC     = process_monitor.c

# ---- Default Target ----
# 'all' is the default target — runs when 'make' is called with no arguments
.PHONY: all
all: $(TARGET)
	@echo ""
	@echo "=============================================="
	@echo "  Build successful! Run with: ./$(TARGET)"
	@echo "=============================================="
	@echo ""

# ---- Build Rule ----
# Compile the C source into an executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

# ---- Run Target ----
# Build and immediately execute
.PHONY: run
run: $(TARGET)
	@echo ""
	@echo "--- Running Process Monitor ---"
	@echo ""
	./$(TARGET)

# ---- Clean Target ----
# Remove the compiled executable and any object files
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET) *.o core
	@echo "Done."

# ---- Check Target (valgrind memory check) ----
# Requires valgrind to be installed: sudo apt install valgrind
.PHONY: check
check: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

# ---- Help Target ----
.PHONY: help
help:
	@echo "Available make targets:"
	@echo "  all     : Build the executable (default)"
	@echo "  run     : Build and run"
	@echo "  clean   : Remove build artifacts"
	@echo "  check   : Run with valgrind (memory check)"
	@echo "  help    : Show this help"
