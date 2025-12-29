// https://hdlbits.01xz.net/wiki/Adder100

module top_module( 
    input [99:0] a, b,
    input cin,
    output cout,
    output [99:0] sum );

    assign {cout, sum} = {{100{1'b0}}, cin} + {1'b0, a} + {1'b0, b};
endmodule
