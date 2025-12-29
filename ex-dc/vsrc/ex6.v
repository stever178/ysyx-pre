`include "mux_template.v"

module ex6(
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

module light_seg_x (
    input [3:0] digit,
    output [6:0] seg
);
    MuxKey #(16, 4, 7) digit2hex (seg, digit, {
        4'h0, 7'b000_0001,
        4'h1, 7'b100_1111,
        4'h2, 7'b001_0010,
        4'h3, 7'b000_0110,

        4'h4, 7'b100_1100,
        4'h5, 7'b010_0100,
        4'h6, 7'b010_0000,
        4'h7, 7'b000_1101,

        4'h8, 7'b000_0000,
        4'h9, 7'b000_0100,
        4'ha, 7'b000_1000,
        4'hb, 7'b110_0000,

        4'hc, 7'b011_0001,
        4'hd, 7'b100_0010,
        4'he, 7'b011_0000,
        4'hf, 7'b011_1000
    });
endmodule

module get_8bit_rnd (
    input        clk,
    input        reset,
    input  [7:0] data_in,
    output [7:0] data_out
);
    reg [7:0] state, next_state;

    always @(*) begin
        next_state = { state[4] ^ state[3] ^ state[2] ^ state[0], state[7:1] };
    end

    always @(posedge clk) begin
        if (reset) begin
            state <= (data_in == 0) ? 8'h01 : data_in;
        end else begin
            state <= next_state;
        end
    end

    assign data_out = state;
endmodule
