
module switch(
	input a,
	input b,
	output f
);
 	assign f = a ^ b;

	initial begin
		$dumpfile("logs/waveform.vcd");
		//$dumpvars(0, switch);
        $dumpvars();
	end
endmodule
