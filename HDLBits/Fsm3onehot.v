// https://hdlbits.01xz.net/wiki/Fsm3onehot

module top_module(
    input in,
    input [3:0] state,
    output [3:0] next_state,
    output out); //

	// One-hot encoding
    // localparam A = 4'b0001;
    // localparam B = 4'b0010;
    // localparam C = 4'b0100;
    // localparam D = 4'b1000;

    // State transition logic: Derive an equation for each state flip-flop.
    assign next_state[0] = ( state[0] & ~in ) | ( state[2] & ~in );          // A
    assign next_state[1] = ( state[0] &  in ) | ( state[1] &  in ) | (state[3] & in); // B
    assign next_state[2] = ( state[1] & ~in ) | ( state[3] & ~in );          // C
    assign next_state[3] = ( state[2] &  in );                               // D

    // Output logic: 
    assign out = state[3];

endmodule
