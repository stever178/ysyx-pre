BUILD_DIR = ./build

BUILD_ABS_DIR = $(abspath $(BUILD_DIR))
NXDC_ABS_DIR = $(abspath ./constr)
CSRC_ABS_DIR = $(abspath ./csrc)
VSRC_ABS_DIR = $(abspath ./vsrc)
SDC_ABS_DIR = $(abspath ./scripts)
CHISEL_FILES_ABS_DIR = $(abspath ./src)
CHISEL_SRCS = $(wildcard $(CHISEL_FILES_ABS_DIR)/main/scala/Ex*.scala)
CHISEL_TESTS = $(wildcard $(CHISEL_FILES_ABS_DIR)/test/scala/Ex*Tester.scala) 

EX_V_FILES := $(wildcard ./vsrc/ex*.v)
EX_SUFFIX := $(sort $(patsubst ./vsrc/ex%.v, %, $(EX_V_FILES)))

EX_TARGETS = $(addprefix build-ex, $(EX_SUFFIX))
RUN_TARGETS = $(addprefix run-ex, $(EX_SUFFIX))
SHOW_TARGETS = $(addprefix show-ex, $(EX_SUFFIX))
GDB_TARGETS = $(addprefix gdb-ex, $(EX_SUFFIX))
STA_TARGETS = $(addprefix sta-ex, $(EX_SUFFIX))

VERILATOR = verilator
VERILATOR_CFLAGS += -MMD --build -cc --trace \
	-O3 --x-assign fast --x-initial fast --noassert
VERILATOR_INC_PATH += $(abspath ./vsrc)
VERILATOR_FLAGS = $(addprefix -I, $(VERILATOR_INC_PATH))

$(shell mkdir -p $(BUILD_DIR))

# constraint file
$(BUILD_DIR)/auto_bind_ex%.cpp: $(NXDC_ABS_DIR)/ex%.nxdc
	python3 $(NVBOARD_HOME)/scripts/auto_pin_bind.py $^ $@

# rules for NVBoard
include $(NVBOARD_HOME)/scripts/nvboard.mk

INCFLAGS = $(addprefix -I, $(INC_PATH))

# Build rules
$(BUILD_DIR)/ex%: $(VSRC_ABS_DIR)/ex%.v $(CSRC_ABS_DIR)/main_ex%.cpp \
				  $(BUILD_DIR)/auto_bind_ex%.cpp $(NVBOARD_ARCHIVE)
	@echo "\nBuilding ex$* ..."
	@rm -rf $(BUILD_DIR)/obj_dir_ex$*
	$(VERILATOR) $(VERILATOR_CFLAGS) $(VERILATOR_FLAGS) \
		--top-module ex$* \
		$(abspath ./vsrc/ex$*.v) \
		$(abspath ./csrc/main_ex$*.cpp) \
		$(abspath $(BUILD_DIR)/auto_bind_ex$*).cpp \
		$(NVBOARD_ARCHIVE) \
		$(addprefix -CFLAGS , $(INCFLAGS) -DTOP_NAME="\"Vex$*\"" $(EXTRA_CFLAGS) -g -O0) \
		$(addprefix -LDFLAGS , $(LDFLAGS)) \
		--Mdir $(BUILD_DIR)/obj_dir_ex$* \
		--exe -o $(abspath $@)

# Static pattern rules
$(EX_TARGETS): build-ex%: $(BUILD_DIR)/ex%

$(RUN_TARGETS): run-ex%: $(BUILD_DIR)/ex%
	@echo "\nRunning ex$* ..."
	@$<

# $(SHOW_TARGETS): EXTRA_CFLAGS = -DUSE_NVBOARD
$(SHOW_TARGETS): show-ex%: $(BUILD_DIR)/ex%
	@echo "\nRunning ex$* (with NVBoard) ..."
	@$< --use_board

$(GDB_TARGETS): gdb-ex%: $(BUILD_DIR)/ex%
	@echo "\nDebugging ex$* with GDB ..."
	gdb $<

$(STA_TARGETS): sta-ex%: $(SDC_ABS_DIR)/ex%.sdc $(VSRC_ABS_DIR)/ex%.v 
	@echo "\nRunning static timing analysis for ex$* ..."
	@make -C $(PRJ)/yosys-sta sta \
		DESIGN=ex$* SDC_FILE=$(SDC_ABS_DIR)/ex$*.sdc \
		CLK_FREQ_MHZ=100 CLK_PORT_NAME=clk O=$(BUILD_ABS_DIR)/result_sta_ex$* \
		RTL_FILES="$(VSRC_ABS_DIR)/ex$*.v"

waves: ./waveform.vcd
	@if [ ! -f "$<" ]; then \
		echo "Error: file $< does not exist."; \
		exit 1; \
	elif [ ! -s "$<" ]; then \
		echo "Error: file $< is empty. Please check your simulation result."; \
		exit 1; \
	else \
		echo "Opening waveform viewer: $< ..."; \
		gtkwave $<; \
	fi

default: $(EX_TARGETS)

TOP = top

TEST_TARGETS = $(addprefix test-ex, $(EX_SUFFIX))
$(TEST_TARGETS): test-ex%: $(CHISEL_FILES_ABS_DIR)/main/scala/Ex%.scala \
						   $(CHISEL_FILES_ABS_DIR)/test/scala/Ex%Tester.scala
	mill -i $(TOP).test -z "Ex$* Module"

VERILOG_TARGETS = $(addprefix verilog-ex, $(EX_SUFFIX))
$(VERILOG_TARGETS): verilog-ex%: $(CHISEL_FILES_ABS_DIR)/main/scala/Ex%.scala
	$(info "Generating verilog for ex$* ...")
	mkdir -p $(BUILD_DIR)
	mill -i $(TOP).runMain ex$*.Ex$*Elaborate --target-dir $(BUILD_DIR)

HELP_TARGETS = $(addprefix help-ex, $(EX_SUFFIX))
$(HELP_TARGETS): help-ex%: $(CHISEL_FILES_ABS_DIR)/main/scala/Ex%.scala
	$(info "Generating help for ex$* ...")
	mill -i $(TOP).runMain ex$*.Ex$*Elaborate --help

reformat:
	mill -i __.reformat

checkformat:
	mill -i __.checkFormat

bsp:
	mill -i mill.bsp.BSP/install

idea:
	mill -i mill.idea.GenIdea/idea

clean:
	rm -rf $(BUILD_DIR) logs waveform.vcd compile_commands.json
	-rm -rf $(BUILD_DIR)

list:
	@echo "\nFound exercises: $(EX_SUFFIX)"

.PHONY: test verilog help reformat checkformat
.PHONY: default all clean waves list \
	$(EX_TARGETS) $(RUN_TARGETS) $(SHOW_TARGETS) $(GDB_TARGETS) $(STA_TARGETS)
