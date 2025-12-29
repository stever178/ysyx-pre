// https://hdlbits.01xz.net/wiki/Conwaylife

module top_module(
    input clk,
    input load,
    input [255:0] data,
    output [255:0] q ); 
    
    function automatic [3:0] count_nghbr(input [16:-1] row_above, row, row_below, input int j);
        count_nghbr =
            row_above[j-1] + row_above[j] + row_above[j+1] +
            row      [j-1] +            0 + row      [j+1] +
            row_below[j-1] + row_below[j] + row_below[j+1];
    endfunction
    
    wire [255+16 : -16] q_ext_row = {q[15:0], q, q[255 -: 16]};
    wire [16:-1] q_2d[16:-1];
    wire [2:0] nghbr_num [255:0];
    
    generate
        genvar i;
        for(i=-1; i<=16; i=i+1) begin : BLOCK0
            assign q_2d[i] = {q_ext_row[i*16], q_ext_row[i*16 +: 16], q_ext_row[i*16 + 15]};
        end
    endgenerate
    
    always@(*) begin
        for(integer i = 0 ; i <= 15; i = i + 1) begin
            for(integer j = 0 ; j <= 15; j = j + 1) begin
                nghbr_num[i*16 + j] = count_nghbr(q_2d[i+1], q_2d[i], q_2d[i-1], j);
            end
        end
    end
    
    reg [255:0] next_q;

    always @(*) begin
        for (int i = 0 ; i < 16; i++)
            for (int j = 0 ; j < 16; j++) begin
                case(nghbr_num[i*16+j])
                    2: next_q[i*16+j] = q[i*16+j];
                    3: next_q[i*16+j] = 1;
                    default: next_q[i*16+j] = 0;
                endcase
            end
    end

    always @(posedge clk) begin
        if(load) q <= data;
        else     q <= next_q;
    end
endmodule

/*
Info: *******************************************************************
Info: Running Quartus Prime Shell
    Info: Version 20.1.1 Build 720 11/11/2020 SJ Lite Edition
    Info: Copyright (C) 2020  Intel Corporation. All rights reserved.
    Info: Your use of Intel Corporation's design tools, logic functions 
    Info: and other software and tools, and any partner logic 
    Info: functions, and any output files from any of the foregoing 
    Info: (including device programming or simulation files), and any 
    Info: associated documentation or information are expressly subject 
    Info: to the terms and conditions of the Intel Program License 
    Info: Subscription Agreement, the Intel Quartus Prime License Agreement,
    Info: the Intel FPGA IP License Agreement, or other applicable license
    Info: agreement, including, without limitation, that your use is for
    Info: the sole purpose of programming logic devices manufactured by
    Info: Intel and sold by Intel or its authorized distributors.  Please
    Info: refer to the applicable agreement for further details, at
    Info: https://fpgasoftware.intel.com/eula.
    Info: Processing started: Mon Dec 29 00:05:29 2025
Info: Command: quartus_sh -t /home/h/hdlbits/compile.tcl
Info: *******************************************************************
Info: Running Quartus Prime Analysis & Synthesis
    Info: Version 20.1.1 Build 720 11/11/2020 SJ Lite Edition
    Info: Processing started: Mon Dec 29 00:05:30 2025
Info: Command: quartus_map --read_settings_files=on --write_settings_files=off top_module -c top_module
Warning (12473): User specified to use only one processors but 4 processors were detected which could be used to decrease run time.
Info (12021): Found 1 design units, including 1 entities, in source file top_module_wrap.v
    Info (12023): Found entity 1: top_module File: /home/h/work/hdlbits.8623198/top_module.v Line: 1
Info (12021): Found 0 design units, including 0 entities, in source file tb_modules.sv
Info (12127): Elaborating entity "top_module" for the top level hierarchy
Warning (10230): Verilog HDL assignment warning at top_module.v(8): truncated value with size 32 to match size of target (4) File: /home/h/work/hdlbits.8623198/top_module.v Line: 8
Warning (10230): Verilog HDL assignment warning at top_module.v(28): truncated value with size 4 to match size of target (3) File: /home/h/work/hdlbits.8623198/top_module.v Line: 28
Info (286030): Timing-Driven Synthesis is running
Info (16010): Generating hard_block partition "hard_block:auto_generated_inst"
    Info (16011): Adding 0 node(s), including 0 DDIO, 0 PLL, 0 transceiver and 0 LCELL
Info (21057): Implemented 2562 device resources after synthesis - the final resource count might be different
    Info (21058): Implemented 258 input pins
    Info (21059): Implemented 256 output pins
    Info (21061): Implemented 2048 logic cells
Info: Quartus Prime Analysis & Synthesis was successful. 0 errors, 3 warnings
    Info: Peak virtual memory: 418 megabytes
    Info: Processing ended: Mon Dec 29 00:05:33 2025
    Info: Elapsed time: 00:00:03
    Info: Total CPU time (on all processors): 00:00:04
Info: *******************************************************************
Info: Running Quartus Prime EDA Netlist Writer
    Info: Version 20.1.1 Build 720 11/11/2020 SJ Lite Edition
    Info: Processing started: Mon Dec 29 00:05:35 2025
Info: Command: quartus_eda --read_settings_files=on --write_settings_files=off top_module -c top_module --write_settings_files=off --functional=on --flatten_buses=off --simulation=on --tool=modelsim_oem --format=verilog --output_directory=.
Info (119006): Selected device 5CSEMA5F31C6 for design "top_module"
Info (204019): Generated file top_module.vo in folder "/home/h/work/hdlbits.8623198/" for EDA simulation tool
Info: Quartus Prime EDA Netlist Writer was successful. 0 errors, 0 warnings
    Info: Peak virtual memory: 679 megabytes
    Info: Processing ended: Mon Dec 29 00:05:35 2025
    Info: Elapsed time: 00:00:00
    Info: Total CPU time (on all processors): 00:00:01
Info (23030): Evaluation of Tcl script /home/h/hdlbits/compile.tcl was successful
Info: Quartus Prime Shell was successful. 0 errors, 3 warnings
    Info: Peak virtual memory: 484 megabytes
    Info: Processing ended: Mon Dec 29 00:05:36 2025
    Info: Elapsed time: 00:00:07
    Info: Total CPU time (on all processors): 00:00:06

Reading pref.tcl

# 2020.1

# do /home/h/hdlbits/runsim.do
# Model Technology ModelSim - Intel FPGA Edition vlog 2020.1 Compiler 2020.02 Feb 28 2020
# Start time: 00:05:37 on Dec 29,2025
# vlog -sv tb.sv "+incdir+../../hdlbits" 
# -- Compiling module reference_module
# -- Compiling module stimulus_gen
# -- Compiling module tb
# -- Compiling module wavedrom_mod
# 
# Top level modules:
# 	tb
# End time: 00:05:37 on Dec 29,2025, Elapsed time: 0:00:00
# Errors: 0, Warnings: 0
# Model Technology ModelSim - Intel FPGA Edition vlog 2020.1 Compiler 2020.02 Feb 28 2020
# Start time: 00:05:37 on Dec 29,2025
# vlog top_module.vo 
# -- Compiling module top_module
# 
# Top level modules:
# 	top_module
# End time: 00:05:37 on Dec 29,2025, Elapsed time: 0:00:00
# Errors: 0, Warnings: 0
# vsim -c -t 1ps -L cyclonev_ver -L altera_ver -L altera_mf_ver -L 220model_ver -L sgate_ver -L altera_lnsim_ver work.tb -voptargs=""+acc"" 
# Start time: 00:05:37 on Dec 29,2025
# Loading sv_std.std
# Loading work.tb
# Loading work.stimulus_gen
# Loading work.reference_module
# Loading work.top_module
# Loading cyclonev_ver.cyclonev_io_obuf
# Loading cyclonev_ver.cyclonev_io_ibuf
# Loading cyclonev_ver.cyclonev_lcell_comb
# Loading altera_ver.dffeas
# Loading work.wavedrom_mod
# Loading altera_ver.PRIM_GDFF_LOW
# ** Warning: Design size of 15279 statements exceeds ModelSim-Intel FPGA Starter Edition recommended capacity.
# Expect performance to be adversely affected.
# ** Note: $finish    : tb.sv(131)
#    Time: 25116 ps  Iteration: 0  Instance: /tb/stim1
# Hint: Output 'q' has no mismatches.
# Hint: Total mismatched samples is 0 out of 5023 samples
# 
# Simulation finished at 25116 ps
# Mismatches: 0 in 5023 samples
# End time: 00:05:45 on Dec 29,2025, Elapsed time: 0:00:08
# Errors: 0, Warnings: 1
*/
