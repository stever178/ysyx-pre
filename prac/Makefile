CFLAGS = -g -Wall -fsanitize=address -O0
CFLAGS += -Werror=shift-count-overflow -Werror=div-by-zero
LFLAGS ?=

BUILD_DIR = ./build

# todo
SOURCES := $(wildcard *.c)

C_SUFFIX := $(patsubst %.c, %, $(SOURCES))

BIN_TARGETS = $(addprefix build-, $(C_SUFFIX))
RUN_TARGETS = $(addprefix run-, $(C_SUFFIX))
GDB_TARGETS = $(addprefix gdb-, $(C_SUFFIX))

$(BUILD_DIR)/%.out: ./%.c
	@echo "Building $* ..."
	@$(shell mkdir -p $(BUILD_DIR))
	@gcc $(CFLAGS) $< -o $@

$(BIN_TARGETS): build-%: $(BUILD_DIR)/%.out

$(RUN_TARGETS): run-%: $(BUILD_DIR)/%.out
	@echo "Running $* ..."
	@$<

$(GDB_TARGETS): gdb-%: $(BUILD_DIR)/%.out
	@echo "Debugging $* with GDB ..."
	@gdb $<

clean:
	@rm -rf $(BUILD_DIR)
	@find . -maxdepth 1 -type f -executable -delete

.PHONY: default all clean \
	$(BIN_TARGETS) $(RUN_TARGETS) $(GDB_TARGETS) 

