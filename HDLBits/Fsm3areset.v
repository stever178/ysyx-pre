// https://hdlbits.01xz.net/wiki/Fsm3

module top_module(
    input clk,
    input in,
    input areset,
    output out); //

	// One-hot encoding
    localparam [3:0] A = 4'b0001;
    localparam [3:0] B = 4'b0010;
    localparam [3:0] C = 4'b0100;
    localparam [3:0] D = 4'b1000;
    
    reg [3:0] state, next_state;
    
    // State transition logic
    assign next_state[0] = ( state[0] & ~in ) | ( state[2] & ~in );          // A
    assign next_state[1] = ( state[0] &  in ) | ( state[1] &  in ) | (state[3] & in); // B
    assign next_state[2] = ( state[1] & ~in ) | ( state[3] & ~in );          // C
    assign next_state[3] = ( state[2] &  in );                               // D
    
    // State flip-flops with asynchronous reset
    always @(posedge clk, posedge areset) begin
        if (areset)
            state <= A;
        else
            state <= next_state;
    end

    // Output logic
    assign out = (state == D);
endmodule
