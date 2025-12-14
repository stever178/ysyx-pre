set CLK_FREQ_MHZ 100

create_clock -name clk -period [expr 1000.0/$CLK_FREQ_MHZ]

foreach port {A_0_ A_1_ A_2_ A_3_ B_0_ B_1_ B_2_ B_3_ sel_0_ sel_1_ sel_2_} {
    set_input_delay -clock clk -max 2.0 [get_ports $port]
    set_input_delay -clock clk -min 0.5 [get_ports $port]
}

foreach port {out_s_0_ out_s_1_ out_s_2_ out_s_3_ out_o out_c out_z} {
    set_output_delay -clock clk -max 2.0 [get_ports $port]
    set_output_delay -clock clk -min 0.5 [get_ports $port]

    set_load -pin_load 0.01 [get_ports $port]
}

# 设置最大延迟路径
# set_max_delay 15.0 -from [get_ports *] -to [get_ports *]
