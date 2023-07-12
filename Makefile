TARGET_EXEC := password-manager

SRC_DIR := src
INCLUDE_DIR := include
SRCS := $(shell find $(SRC_DIR) -type f -name '*.cpp')

CXX := g++
CXXFLAGS := -I $(INCLUDE_DIR)

BUILD_DIR := build
DEP_DIR := $(BUILD_DIR)/.deps
DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_DIR)/$*.Td
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

%.o: %.cpp # Delete the default implicit rule.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d
	mkdir -p $(dir $@)
	$(CXX) $(DEP_FLAGS) $(CXXFLAGS) -c $< -o $@

# During compilation dependencies are written to a temporary file in order to
# avoid generating broken dependency files or incorrect timestamps.
# https://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# Move them to a non-temporary file only after the compilation has
# succeeded.
	mv -f $(DEP_DIR)/$*.Td $(DEP_DIR)/$*.d
	touch $@

DEP_FILES := $(SRCS:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)
$(DEP_FILES):
	mkdir -p $(dir $@)

include $(wildcard $(DEP_FILES))

run: $(BUILD_DIR)/$(TARGET_EXEC)
	$(BUILD_DIR)/$(TARGET_EXEC)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
