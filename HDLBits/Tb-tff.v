// https://hdlbits.01xz.net/wiki/Tb/tff

module top_module (  );
	parameter [3:0] unit = 4'd5;

    reg clk;
    reg reset;
    reg t;
    reg q;

	tff dut(clk, reset, t, q);

	initial begin
		forever begin
			clk = 1'b0; #unit;
			clk = 1'b1; #unit;
		end
	end
    
    initial begin
        reset = 1'b1; #(unit * 2);
		reset = 1'b0;
    end

	initial begin
        t = 1'b0;
        wait(reset == 1'b0); #10;
        t = 1'b1; #30;
        t = 1'b0;
	end

	initial begin
        $monitor("Time=%0t: clk=%b, reset=%b, t=%b, q=%b", $time, clk, reset, t, q);
		#100;
		$finish;
    end

endmodule
