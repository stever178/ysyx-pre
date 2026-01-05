// https://nju-projectn.github.io/dlco-lecture-note/exp/06.html#id5

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
	// A和B都已经是补码
    wire [4:0] add_result = {1'b0, A} + {1'b0, B};
    wire [4:0] sub_result = {1'b0, A} + {1'b0, ~B} + 5'b00001;
	reg flag_o_sub;

	always @(*) begin
		flag_o_sub = 1'b0;
		out_o = 1'b0;
	    out_c = 1'b0;
    	out_s = 4'b0;

		case (sel)
			// 加法运算: {carry,sum} = A + B
			3'b000: begin
				{out_c, out_s} = add_result;
				out_o = (A[3] == B[3]) && (add_result[3] != A[3]);
			end
			
			// 减法运算: method2, carry flag <-> "no borrow flag"
			3'b001: begin
				{out_c, out_s} = sub_result;
				out_o = (A[3] != B[3]) && (sub_result[3] != A[3]);
			end

			// 按位取反
			3'b010: begin
				out_s = ~A;
			end
			
			// 按位与运算
			3'b011: begin
				out_s = A & B;
			end
			
			// 按位或运算
			3'b100: begin
				out_s = A | B;
			end
			
			// 按位异或运算
			3'b101: begin
				out_s = A ^ B;
			end

			// 比较运算 (A < B)
			3'b110: begin
				flag_o_sub = (A[3] != B[3]) && (sub_result[3] != A[3]);
				out_s = {3'b0, (flag_o_sub ^ sub_result[3])};
			end
			
			// 相等运算 (A == B)
			3'b111: begin
				out_s = {3'b0, ~(| sub_result[3:0])};
			end
		endcase
	end

	assign out_z = ~(| out_s);
endmodule
