// https://hdlbits.01xz.net/wiki/Exams/2014_q4b

module top_module (
    input [3:0] SW,
    input [3:0] KEY,
    output [3:0] LEDR
); //
    wire clk = KEY[0];
    wire ena = KEY[1];
    wire load = KEY[2];
    wire w = KEY[3];
    wire [3:0] R = SW;
    
    MUXDFF d3 (clk, ena, load, R[3], w, LEDR[3]);
    MUXDFF d2 (clk, ena, load, R[2], LEDR[3], LEDR[2]);
    MUXDFF d1 (clk, ena, load, R[1], LEDR[2], LEDR[1]);
    MUXDFF d0 (clk, ena, load, R[0], LEDR[1], LEDR[0]);
    
endmodule

module MUXDFF (
    input clk,
	input ena,
    input load,
    input R,
    input data,
    output out
);
    always @(posedge clk) begin
        if (load) 
            out <= R;
        else begin
            if (ena)
                out <= data;
            else 
                out <= out;
        end
    end
endmodule
