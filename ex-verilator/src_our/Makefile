BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj_dir

VSRCS = $(shell find $(abspath ./) -name "*.v")
CSRCS = $(shell find $(abspath ./) -name "*.cpp")

build: $(VSRCS) $(CSRCS)
	@mkdir -p $(OBJ_DIR)
	verilator --cc --exe --build -j 0 -Wall $^ \
		--Mdir $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BUILD_DIR)
	@rm -f compile_commands.json
