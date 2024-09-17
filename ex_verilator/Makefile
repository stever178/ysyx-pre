
VERILATOR_FLAGS += --trace

switch: sim_switch.cpp switch.v
	verilator --cc --exe --build -j 0 -Wall --Mdir obj_switch $(VERILATOR_FLAGS) $^


clean:
	rm -rf obj_*
