# 对于纯组合逻辑，可以只定义输入输出环境约束
# 不创建实际时钟

# 直接设置输入转换时间
set_input_transition 0.1 [all_inputs]

# 设置输出负载
set_load 0.02 [all_outputs]


# 可选：设置输入延迟
set_input_delay -clock [clock_name] [input_delay] [all_inputs]

# 可选：设置输出延迟
set_output_delay -clock [clock_name] [output_delay] [all_outputs]

# 可选：设置驱动模型和负载
set_driving_cell [driver_cell]
set_load [load_capacitance] [outputs]


# 设置时钟约束
set CLK_PORT_NAME [clock_port_name]
set CLK_FREQ_MHZ [frequency_in_mhz]
set clk_period [expr 1000.0 / $CLK_FREQ_MHZ]

# 创建时钟
create_clock -name [clock_name] -period $clk_period [get_ports $CLK_PORT_NAME]

# 定义虚拟时钟
create_clock -name virtual_clk -period 10.0


# 特定端口的输入延迟
set_input_delay -clock virtual_clk -max 3.0 [get_ports {data_in[*]}]
set_input_delay -clock virtual_clk -min 0.5 [get_ports {data_in[*]}]

# 特定端口的输出延迟
set_output_delay -clock virtual_clk -max 2.5 [get_ports {data_out[*]}]
set_output_delay -clock virtual_clk -min 0.8 [get_ports {data_out[*]}]

# 设置多周期路径
set_multicycle_path -setup 2 -from [get_ports slow_input] -to [get_registers fast_reg]
