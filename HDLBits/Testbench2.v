// https://hdlbits.01xz.net/wiki/Tb/tb2

module top_module (  );
    reg clk;
    reg in;
    reg [2:0] s;
    reg out;

	q7 dut(clk, in, s, out);

	initial begin
		forever begin
			clk = 1'b0; #5;
			clk = 1'b1; #5;
		end
	end
    
    initial begin
        in = 1'b0; #20;
		in = 1'b1; #10;
		in = 1'b0; #10;
		in = 1'b1; #30;
		in = 1'b0;
    end

	initial begin
		s = 3'd2; #10;
		s = 3'd6; #10;
		s = 3'd2; #10;
		s = 3'd7; #10;
		s = 3'd0;
	end

endmodule
