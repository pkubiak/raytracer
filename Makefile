CC=g++
LD=g++
CCFLAGS=-Isrc -c -std=c++11
LDFLAGS=-lpng
debug : CCFLAGS += -g -Wall -Wextra -Wno-unused-parameter -Wno-unknown-pragmas -Wno-sign-compare
release : CCFLAGS += -O3 -DNDEBUG -fopenmp
release : LDFLAGS += -O3 -fopenmp
SRC_DIR:=src
INT_DIR_DEBUG:=build/debug
INT_DIR_RELEASE:=build/release
OUT_DIR=bin
TARGET_DEBUG=cgrayd
TARGET_RELEASE=cgray

SOURCES := $(shell find $(SRC_DIR) -name *.cpp)
SRCDIRS := $(shell find $(SRC_DIR) -type d)

DIRS_DEBUG = $(SRCDIRS:$(SRC_DIR)%=$(INT_DIR_DEBUG)%)
DIRS_RELEASE = $(SRCDIRS:$(SRC_DIR)%=$(INT_DIR_RELEASE)%)

OBJS_DEBUG = $(SOURCES:$(SRC_DIR)/%.cpp=$(INT_DIR_DEBUG)/%.o)
OBJS_RELEASE = $(SOURCES:$(SRC_DIR)/%.cpp=$(INT_DIR_RELEASE)/%.o)
DEPS_DEBUG = $(SOURCES:$(SRC_DIR)/%.cpp=$(INT_DIR_DEBUG)/%.d)
DEPS_RELEASE = $(SOURCES:$(SRC_DIR)/%.cpp=$(INT_DIR_RELEASE)/%.d)

all: debug

debug: $(OUT_DIR)/$(TARGET_DEBUG)

release: $(OUT_DIR)/$(TARGET_RELEASE)

$(OUT_DIR)/$(TARGET_DEBUG) : $(OBJS_DEBUG) | $(DIRS_DEBUG) $(OUT_DIR)
	$(LD) $^ $(LDFLAGS) -o $@

$(OUT_DIR)/$(TARGET_RELEASE) : $(OBJS_RELEASE) | $(DIRS_RELEASE) $(OUT_DIR)
	$(LD) $^ $(LDFLAGS) -o $@

$(INT_DIR_DEBUG)/%.o : $(SRC_DIR)/%.cpp $(INT_DIR_DEBUG)/%.d | $(DIRS_DEBUG)
	$(CC) $< $(CCFLAGS) -c -o $@

$(INT_DIR_RELEASE)/%.o : $(SRC_DIR)/%.cpp $(INT_DIR_RELEASE)/%.d | $(DIRS_RELEASE)
	$(CC) $< $(CCFLAGS) -c -o $@

$(INT_DIR_DEBUG)/%.d : $(SRC_DIR)/%.cpp | $(DIRS_DEBUG)
	@$(CC) $< -MM -MP |\
		sed 's=\($(*F)\)\.o[ :]*=$(@D)/\1.o $@ : =g;'\
		> $@

$(INT_DIR_RELEASE)/%.d : $(SRC_DIR)/%.cpp | $(DIRS_RELEASE)
	@$(CC) $< -MM -MP |\
		sed 's=\($(*F)\)\.o[ :]*=$(@D)/\1.o $@ : =g;'\
		> $@

$(DIRS_DEBUG) $(DIRS_RELEASE) $(OUT_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(INT_DIR_DEBUG)
	@rm -rf $(INT_DIR_RELEASE)
	@rm -rf $(OUT_DIR)

.PHONY: all debug release clean
.SECONDARY: $(OBJS_DEBUG) $(OBJS_RELEASE) $(DEPS_DEBUG) $(DEPS_RELEASE)

-include $(DEPS)
