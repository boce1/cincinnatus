# Compilers
CC_LINUX = gcc
CXX_LINUX = g++
CC_WIN   = x86_64-w64-mingw32-gcc
CXX_WIN  = x86_64-w64-mingw32-g++

# Directories
SRC_DIR  = src
NNUE_DIR = $(SRC_DIR)/nnue
BIN_DIR  = bin

# Sources and executables
SRC_C     = $(wildcard $(SRC_DIR)/*.c)
SRC_CPP   = $(wildcard $(NNUE_DIR)/*.cpp)
SRC_ALL   = $(SRC_C) $(SRC_CPP)
OBJ_ROOT = obj
OBJ_LIN_DIR = $(OBJ_ROOT)/linux
OBJ_LIN_NNUE_DIR = $(OBJ_LIN_DIR)/nnue
OBJ_WIN_DIR = $(OBJ_ROOT)/windows
OBJ_WIN_NNUE_DIR = $(OBJ_WIN_DIR)/nnue
OBJS      = $(patsubst $(SRC_DIR)/%.c,$(OBJ_LIN_DIR)/%.o,$(SRC_C)) \
			$(patsubst $(NNUE_DIR)/%.cpp,$(OBJ_LIN_NNUE_DIR)/%.o,$(SRC_CPP))
EXEC_LIN  = $(BIN_DIR)/cincinnatus_linux
EXEC_WIN  = $(BIN_DIR)/cincinnatus_windows
WIN_OBJ_DIR = $(OBJ_DIR)/windows

# Compiler flags
CFLAGS_DEBUG   = -Wall -Wextra
CFLAGS_RELEASE = -Ofast -fomit-frame-pointer

# NNUE / SIMD flags (as used in bbc)
NNUE_FLAGS = -DUSE_SSE41 -msse4.1 -DUSE_SSSE3 -mssse3 -DUSE_SSE2 -msse2 -DUSE_SSE -msse

# Default target: build releases
all: linux_release windows_release

# ---- Linux ----
linux_release: $(EXEC_LIN)_release
linux_debug:   $(EXEC_LIN)_debug

$(EXEC_LIN)_release: CFLAGS := $(CFLAGS_RELEASE)
$(EXEC_LIN)_release: $(OBJS)
	$(CXX_LINUX) $(CFLAGS) -o $@ $^

$(EXEC_LIN)_debug: CFLAGS := $(CFLAGS_DEBUG)
$(EXEC_LIN)_debug: $(OBJS)
	$(CXX_LINUX) $(CFLAGS) -o $@ $^

# ---- Windows ----
windows_release: $(EXEC_WIN)_release
windows_debug:   $(EXEC_WIN)_debug

$(EXEC_WIN)_release: CFLAGS := $(CFLAGS_RELEASE)
$(EXEC_WIN)_release:
	@mkdir -p $(OBJ_WIN_DIR) $(OBJ_WIN_NNUE_DIR)
	@echo "Compiling Windows .c sources with $(CC_WIN)"
	for f in $(SRC_C); do \
		base=$$(basename $$f .c); \
		$(CC_WIN) $$(echo $(CFLAGS) $(NNUE_FLAGS)) -c -o $(OBJ_WIN_DIR)/$$base.o $$f || exit 1; \
	done
	@echo "Compiling Windows .cpp sources with $(CXX_WIN)"
	for f in $(SRC_CPP); do \
		base=$$(basename $$f .cpp); \
		$(CXX_WIN) $$(echo $(CFLAGS) $(NNUE_FLAGS)) -c -o $(OBJ_WIN_NNUE_DIR)/$$base.o $$f || exit 1; \
	done
	@echo "Linking Windows executable"
	$(CXX_WIN) $(CFLAGS) $(NNUE_FLAGS) -o $@ $(OBJ_WIN_DIR)/*.o $(OBJ_WIN_NNUE_DIR)/*.o

$(EXEC_WIN)_debug: CFLAGS := $(CFLAGS_DEBUG)
$(EXEC_WIN)_debug:
	@mkdir -p $(OBJ_WIN_DIR) $(OBJ_WIN_NNUE_DIR)
	@echo "Compiling Windows .c sources with $(CC_WIN)"
	for f in $(SRC_C); do \
		base=$$(basename $$f .c); \
		$(CC_WIN) $$(echo $(CFLAGS) $(NNUE_FLAGS)) -c -o $(OBJ_WIN_DIR)/$$base.o $$f || exit 1; \
	done
	@echo "Compiling Windows .cpp sources with $(CXX_WIN)"
	for f in $(SRC_CPP); do \
		base=$$(basename $$f .cpp); \
		$(CXX_WIN) $$(echo $(CFLAGS) $(NNUE_FLAGS)) -c -o $(OBJ_WIN_NNUE_DIR)/$$base.o $$f || exit 1; \
	done
	@echo "Linking Windows executable"
	$(CXX_WIN) $(CFLAGS) $(NNUE_FLAGS) -o $@ $(OBJ_WIN_DIR)/*.o $(OBJ_WIN_NNUE_DIR)/*.o


# ---- Build rules ----
$(OBJ_LIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_LIN_DIR)
	$(CC_LINUX) $(CFLAGS) $(NNUE_FLAGS) -c -o $@ $<

$(OBJ_LIN_NNUE_DIR)/%.o: $(NNUE_DIR)/%.cpp
	@mkdir -p $(OBJ_LIN_NNUE_DIR)
	$(CXX_LINUX) $(CFLAGS) $(NNUE_FLAGS) -c -o $@ $<

# ---- Debug ----
debug: linux_debug windows_debug

# ---- Cleanup ----
clean:
	rm -f $(BIN_DIR)/* $(OBJ_DIR)/* $(OBJ_NNUE_DIR)/* $(WIN_OBJ_DIR)/*

.PHONY: all linux_release linux_debug windows_release windows_debug debug clean

