# Compilers
CC_LINUX = gcc
CC_WIN   = x86_64-w64-mingw32-gcc

# Directories
SRC_DIR  = src
BIN_DIR  = bin

# Sources and executables
SRC       = $(wildcard $(SRC_DIR)/*.c)
EXEC_LIN  = $(BIN_DIR)/cincinnatus_linux
EXEC_WIN  = $(BIN_DIR)/cincinnatus_windows

# Compiler flags
CFLAGS_DEBUG   = -Wall -Wextra
CFLAGS_RELEASE = -Ofast

# Default target: build releases
all: linux_release windows_release

# ---- Linux ----
linux_release: $(EXEC_LIN)_release
linux_debug:   $(EXEC_LIN)_debug

$(EXEC_LIN)_release: $(SRC)
	$(CC_LINUX) $(CFLAGS_RELEASE) -o $@ $^

$(EXEC_LIN)_debug: $(SRC)
	$(CC_LINUX) $(CFLAGS_DEBUG) -o $@ $^

# ---- Windows ----
windows_release: $(EXEC_WIN)_release
windows_debug:   $(EXEC_WIN)_debug

$(EXEC_WIN)_release: $(SRC)
	$(CC_WIN) $(CFLAGS_RELEASE) -o $@ $^

$(EXEC_WIN)_debug: $(SRC)
	$(CC_WIN) $(CFLAGS_DEBUG) -o $@ $^


# ---- Debug ----
debug: linux_debug windows_debug

# ---- Cleanup ----
clean:
	rm -f $(BIN_DIR)/*

