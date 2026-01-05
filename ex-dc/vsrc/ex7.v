// https://nju-projectn.github.io/dlco-lecture-note/exp/07.html#id10

`include "mux_template.v"

module ex7(
    input clk,
    input reset,
    input [7:0] data_in,
    output [7:0] data_out,
	output reg [6:0] seg0, seg1
);
    get_8bit_rnd shift0 (
        .clk(clk),
        .reset(reset),
        .data_in(data_in),
        .data_out(data_out)
    );

    // 8'h??
    light_seg_x num0 (
        .digit(data_out[3:0]),
        .seg(seg0)
    );
    light_seg_x num1 (
        .digit(data_out[7:4]),
        .seg(seg1)
    );
endmodule

