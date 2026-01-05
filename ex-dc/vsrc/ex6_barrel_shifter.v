// https://nju-projectn.github.io/dlco-lecture-note/exp/06.html#id4
// https://www.wikiwand.com/zh-hans/articles/%E6%A1%B6%E5%BC%8F%E7%A7%BB%E4%BD%8D%E5%99%A8

/*
 选择端L/R表示左移和右移，置为1为左移，置为0为右移。 
 选择端A/L为算术逻辑选择，置为1为算术移位，置为0为逻辑移位。
*/
module ex6_barrel_shifter(
	input LRw,
	input ALw,
	input [2:0] shamt,
    input [7:0] data_in,
    output [7:0] data_out
); 
	reg [7:0] data_t1, data_t2, data_t3;

	always @(*) begin
		if (LRw == 1'b1) begin
			data_t1 = shamt[2] ? {data_in[3:0], 4'b0} : data_in;
			data_t2 = shamt[1] ? {data_t1[5:0], 2'b0} : data_t1;
			data_t3 = shamt[0] ? {data_t2[6:0], 1'b0} : data_t2;
		end else begin
			if (ALw == 1'b1) begin
				data_t1 = shamt[2] ? {{4{data_in[7]}}, data_in[7:4]} : data_in;
				data_t2 = shamt[1] ? {{2{data_t1[7]}}, data_t1[7:2]} : data_t1;
				data_t3 = shamt[0] ? {{1{data_t2[7]}}, data_t2[7:1]} : data_t2;
			end else begin
				data_t1 = shamt[2] ? {4'b0, data_in[7:4]} : data_in;
				data_t2 = shamt[1] ? {2'b0, data_t1[7:2]} : data_t1;
				data_t3 = shamt[0] ? {1'b0, data_t2[7:1]} : data_t2;
			end
		end
	end

	assign data_out = data_t3;	

endmodule
