set CLK_FREQ_MHZ 100

create_clock -name clk -period [expr 1000.0/$CLK_FREQ_MHZ]

# 设置输入延迟
foreach port {x_0_ x_1_ x_2_ x_3_ x_4_ x_5_ x_6_ x_7_ EN} {
    set_input_delay -clock clk -max 2.0 [get_ports $port]
    set_input_delay -clock clk -min 0.5 [get_ports $port]
}

# 设置输出延迟
foreach port {y_0_ y_1_ y_2_ seg_0_ seg_1_ seg_2_ seg_3_ seg_4_ seg_5_ seg_6_} {
    set_output_delay -clock clk -max 2.0 [get_ports $port]
    set_output_delay -clock clk -min 0.5 [get_ports $port]
}

# 设置负载
foreach port {y_0_ y_1_ y_2_ seg_0_ seg_1_ seg_2_ seg_3_ seg_4_ seg_5_ seg_6_} {
    set_load -pin_load 0.01 [get_ports $port]
}

# 设置最大延迟路径
# set_max_delay 15.0 -from [get_ports *] -to [get_ports *]
