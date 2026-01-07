// https://hdlbits.01xz.net/wiki/Tb/tb1

module top_module ( output reg A, output reg B );//

    // generate input patterns here
    initial begin
		A = 1'b0; #10;
        A = 1'b1; #10;
        A = 1'b0;
    end
    
    initial begin
        B = 1'b0; #15;
        B = 1'b1; #25;
        B = 1'b0;
    end

endmodule
