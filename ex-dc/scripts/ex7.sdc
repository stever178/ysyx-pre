set CLK_FREQ_MHZ 100

create_clock -name clk -period [expr 1000.0/$CLK_FREQ_MHZ]

# 输入端口设置
foreach port {kbd_data clrn} {
    set_input_delay -clock clk -max 2.0 [get_ports $port]
    set_input_delay -clock clk -min 0.5 [get_ports $port]
}

# 输出端口设置
# set_output_delay -clock virtual_clk -max 3.0 [all_outputs]

# set_output_delay -clock virtual_clk -max 2.0 [get_ports {seg_empty0[*]}]
set_output_delay -clock virtual_clk -max 2.0 [get_ports {seg_empty0_0_}]
set_output_delay -clock virtual_clk -min 0.5 [get_ports {seg_empty0_0_}]

# foreach port {out_s_0_ out_s_1_ out_s_2_ out_s_3_ out_o out_c out_z} {
# foreach port {
# 		ready overflow sampling 
# 		ps2_out[7:0] ascii_out[7:0] 
# 		seg_count1[6:0] seg_count0[6:0] 
# 		seg_ascii1[6:0] seg_ascii0[6:0] 
# 		seg_scan_code1[6:0] seg_scan_code0[6:0] 
# 		seg_empty1[6:0] seg_empty0[6:0]} {
#     set_output_delay -clock clk -max 2.0 [get_ports $port]
#     set_output_delay -clock clk -min 0.5 [get_ports $port]

#     set_load -pin_load 0.01 [get_ports $port]
# }
