# ---------------------------------------------------------------------------
# Compilers
CC_LINUX = gcc
CXX_LINUX = g++
CC_WIN   = x86_64-w64-mingw32-gcc
CXX_WIN  = x86_64-w64-mingw32-g++
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Directories
SRC_DIR  = src
NNUE_DIR = $(SRC_DIR)/nnue
BIN_DIR  = bin
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# Sources and executables
SRC_C     = $(wildcard $(SRC_DIR)/*.c)
SRC_CPP   = $(wildcard $(NNUE_DIR)/*.cpp)
SRC_ALL   = $(SRC_C) $(SRC_CPP)
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# Object files
OBJ_ROOT = obj

OBJ_LIN_DIR = $(OBJ_ROOT)/linux
OBJ_LIN_NNUE_DIR = $(OBJ_LIN_DIR)/nnue

OBJ_WIN_DIR = $(OBJ_ROOT)/windows
OBJ_WIN_NNUE_DIR = $(OBJ_WIN_DIR)/nnue

OBJS_LIN  = $(patsubst $(SRC_DIR)/%.c,$(OBJ_LIN_DIR)/%.o,$(SRC_C)) \
			$(patsubst $(NNUE_DIR)/%.cpp,$(OBJ_LIN_NNUE_DIR)/%.o,$(SRC_CPP))

# Windows object lists (cross-compiled)
OBJS_WIN  = $(patsubst $(SRC_DIR)/%.c,$(OBJ_WIN_DIR)/%.o,$(SRC_C)) \
			$(patsubst $(NNUE_DIR)/%.cpp,$(OBJ_WIN_NNUE_DIR)/%.o,$(SRC_CPP))
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# Executable names
EXEC_LIN  = $(BIN_DIR)/cincinnatus_linux
EXEC_WIN  = $(BIN_DIR)/cincinnatus_windows
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# Compiler flags
CFLAGS_DEBUG   = -Wall -Wextra
CFLAGS_RELEASE = -Ofast -fomit-frame-pointer

# NNUE / SIMD flags (as used in bbc)
NNUE_FLAGS = -DUSE_SSE41 -msse4.1 -DUSE_SSSE3 -mssse3 -DUSE_SSE2 -msse2 -DUSE_SSE -msse
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# Default target: build releases
all: linux_release windows_release
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# THIS IS CREATING EXECUTABLE STEP
# ---- Linux ----
linux_release: $(EXEC_LIN)_release
linux_debug:   $(EXEC_LIN)_debug

$(EXEC_LIN)_release: CFLAGS := $(CFLAGS_RELEASE)
$(EXEC_LIN)_release: $(OBJS_LIN)
	@mkdir -p $(BIN_DIR)
	$(CXX_LINUX) $(CFLAGS) $(NNUE_FLAGS) -o $@ $(OBJS_LIN)

$(EXEC_LIN)_debug: CFLAGS := $(CFLAGS_DEBUG)
$(EXEC_LIN)_debug: $(OBJS_LIN)
	@mkdir -p $(BIN_DIR)
	$(CXX_LINUX) $(CFLAGS) $(NNUE_FLAGS) -o $@ $(OBJS_LIN)

# ---- Windows ----
windows_release: $(EXEC_WIN)_release
windows_debug:   $(EXEC_WIN)_debug

$(EXEC_WIN)_release: CFLAGS := $(CFLAGS_RELEASE)
$(EXEC_WIN)_release: $(OBJS_WIN)
	@mkdir -p $(BIN_DIR)
	$(CXX_WIN) $(CFLAGS) $(NNUE_FLAGS) -o $@ $(OBJS_WIN)

$(EXEC_WIN)_debug: CFLAGS := $(CFLAGS_DEBUG)
$(EXEC_WIN)_debug: $(OBJS_WIN)
	@mkdir -p $(BIN_DIR)
	$(CXX_WIN) $(CFLAGS) $(NNUE_FLAGS) -o $@ $(OBJS_WIN)
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# THIS PRODUCES OBJECT FILES
# ---- Build rules ----
$(OBJ_LIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_LIN_DIR)
	$(CC_LINUX) $(CFLAGS) $(NNUE_FLAGS) -c -o $@ $<

$(OBJ_LIN_NNUE_DIR)/%.o: $(NNUE_DIR)/%.cpp
	@mkdir -p $(OBJ_LIN_NNUE_DIR)
	$(CXX_LINUX) $(CFLAGS) $(NNUE_FLAGS) -c -o $@ $<

$(OBJ_WIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_WIN_DIR)
	$(CC_WIN) $(CFLAGS) $(NNUE_FLAGS) -c -o $@ $<

$(OBJ_WIN_NNUE_DIR)/%.o: $(NNUE_DIR)/%.cpp
	@mkdir -p $(OBJ_WIN_NNUE_DIR)
	$(CXX_WIN) $(CFLAGS) $(NNUE_FLAGS) -c -o $@ $<
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# ---- Debug ----
debug: linux_debug windows_debug
# ---------------------------------------------------------------------------


# ---------------------------------------------------------------------------
# ---- Cleanup ----
clean:
	rm -rf $(OBJ_ROOT)/*

.PHONY: all linux_release linux_debug windows_release windows_debug debug clean
# ---------------------------------------------------------------------------
