BUILD_DIR = ./build

BUILD_ABS_DIR = $(abspath $(BUILD_DIR))
VSRC_ABS_DIR = $(abspath ./vsrc)
SDC_ABS_DIR = $(abspath ./scripts)

EX_V_FILES := $(wildcard ./vsrc/ex*.v)
EX_SUFFIX := $(sort $(patsubst ./vsrc/ex%.v, %, $(EX_V_FILES)))

EX_TARGETS = $(addprefix build-ex, $(EX_SUFFIX))
RUN_TARGETS = $(addprefix run-ex, $(EX_SUFFIX))
GDB_TARGETS = $(addprefix gdb-ex, $(EX_SUFFIX))
STA_TARGETS = $(addprefix sta-ex, $(EX_SUFFIX))

VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc --trace \
	-O3 --x-assign fast --x-initial fast --noassert
VERILATOR_INC_PATH += $(abspath ./vsrc)
VERILATOR_FLAGS = $(addprefix -I, $(VERILATOR_INC_PATH))

$(shell mkdir -p $(BUILD_DIR))

# constraint file
$(BUILD_DIR)/auto_bind_ex%.cpp: constr/ex%.nxdc
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# rules for NVBoard
include $(NVBOARD_HOME)/scripts/nvboard.mk

INCFLAGS = $(addprefix -I, $(INC_PATH))

# Build rules
$(BUILD_DIR)/ex%: ./vsrc/ex%.v ./csrc/main_ex%.cpp $(BUILD_DIR)/auto_bind_ex%.cpp $(NVBOARD_ARCHIVE)
	@echo "Building ex$* ..."
	@rm -rf $(BUILD_DIR)/obj_dir_ex$*
	$(VERILATOR) $(VERILATOR_CFLAGS) $(VERILATOR_FLAGS) \
		--top-module ex$* \
		$(abspath ./vsrc/ex$*.v) \
		$(abspath ./csrc/main_ex$*.cpp) \
		$(abspath $(BUILD_DIR)/auto_bind_ex$*).cpp \
		$(NVBOARD_ARCHIVE) \
		$(addprefix -CFLAGS , $(INCFLAGS) -DTOP_NAME="\"Vex$*\"" -g -O0) \
		$(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(BUILD_DIR)/obj_dir_ex$* \
		--exe -o $(abspath $@)

default: $(EX_TARGETS)

# Static pattern rules
$(EX_TARGETS): build-ex%: $(BUILD_DIR)/ex%

$(RUN_TARGETS): run-ex%: $(BUILD_DIR)/ex%
	@echo "Running ex$* ..."
	@$<

$(GDB_TARGETS): gdb-ex%: $(BUILD_DIR)/ex%
	@echo "Debugging ex$* with GDB ..."
	gdb $<

$(STA_TARGETS): sta-ex%: ./constr/ex%.sdc ./vsrc/ex%.v 
	@echo "Running static timing analysis for ex$* ..."
	@make -C $(PRJ)/yosys-sta sta \
		DESIGN=ex$* SDC_FILE=$(SDC_ABS_DIR)/ex$*.sdc \
		CLK_FREQ_MHZ=100 CLK_PORT_NAME=clk O=$(BUILD_ABS_DIR)/result_sta_ex$* \
		RTL_FILES="$(VSRC_ABS_DIR)/ex$*.v"

waves: waveform.vcd
	@gtkwave waveform.vcd

clean:
	@rm -rf $(BUILD_DIR) logs waveform.vcd compile_commands.json

list:
	@echo "Found exercises: $(EX_SUFFIX)"

.PHONY: default all clean waves list \
	$(EX_TARGETS) $(RUN_TARGETS) $(GDB_TARGETS) $(STA_TARGETS)
