// https://nju-projectn.github.io/dlco-lecture-note/exp/06.html#id5

`include "mux_template.v"

module ex3(
	input [3:0] A,
	input [3:0] B,
	input [2:0] sel,
	output reg out_o,
	output reg out_c,
	output reg [3:0] out_s,
	output reg out_z
);
	alu_4bit alu0 (
		.A(A),
		.B(B),
		.sel(sel),
		.out_o(out_o),
		.out_c(out_c),
		.out_s(out_s),
		.out_z(out_z)
	);
endmodule


module alu_4bit(
	input [3:0] A,
	input [3:0] B,
	input [2:0] sel,
	output reg out_o,
	output reg out_c,
	output reg [3:0] out_s,
	output reg out_z
);
    wire [4:0] add_result = {1'b0, A} + {1'b0, B};
    wire [4:0] sub_result = {1'b0, A} + {1'b0, ~B} + 5'b00001;
	wire flag_o_sub = (A[3] != B[3]) && (sub_result[3] != A[3]);

    MuxKeyWithDefault #(8, 3, 4) mux_s (out_s, sel, 4'b0, {
	    3'b000, add_result[3:0],
	    3'b001, sub_result[3:0],
	    3'b010, ~A,
	    3'b011, A & B,
	    3'b100, A | B,
	    3'b101, A ^ B,
	    3'b110, {3'b0, (flag_o_sub ^ sub_result[3])},
	    3'b111, {3'b0, ~(| sub_result[3:0])}
    });
	
    MuxKeyWithDefault #(2, 3, 1) mux_c (out_c, sel, 1'b0, {
	    3'b000, add_result[4],
	    3'b001, sub_result[4]
    });

    MuxKeyWithDefault #(2, 3, 1) mux_o (out_o, sel, 1'b0, {
	    3'b000, (A[3] == B[3]) && (add_result[3] != A[3]),
	    3'b001, (A[3] != B[3]) && (sub_result[3] != A[3])
    });

	assign out_z = ~(| out_s);
endmodule
