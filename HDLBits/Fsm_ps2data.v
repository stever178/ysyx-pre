// https://hdlbits.01xz.net/wiki/Fsm_ps2data

/*
 * out_bytes[23:16] is the first byte, out_bytes[15:8] is the second byte, etc.
 */
module top_module(
    input clk,
    input [7:0] in,
    input reset,    // Synchronous reset
    output [23:0] out_bytes,
    output done); //
    
    parameter START = 2'd0;
    parameter BYTE1 = 2'd1;
    parameter BYTE2 = 2'd2;
    parameter BYTE3 = 2'd3;
    
    reg [1:0] state;
    reg [1:0] next_state;
    reg [7:0] bytes[3:1];

    assign out_bytes = {bytes[1], bytes[2], bytes[3]};

    // State transition logic (combinational)
    always@(*) begin
        case(state)
            START: next_state = in[3] ? BYTE1 : START;
            BYTE1: next_state = BYTE2;
            BYTE2: next_state = BYTE3;
            BYTE3: next_state = in[3] ? BYTE1 : START;
        endcase
	end

    always @(posedge clk) begin
        if(reset) begin
            bytes[3] <= 8'h00;
            bytes[2] <= 8'h00;
            bytes[1] <= 8'h00;
        end else begin
            if ((state == START || state == BYTE3) & in[3])
                bytes[1] <= in;
            else
                bytes[1] <= bytes[1];
            
            if (state == BYTE1)
                bytes[2] <= in;
            else
                bytes[2] <= bytes[2];
            
            if (state == BYTE2)
                bytes[3] <= in;
            else
                bytes[3] <= bytes[3];
        end
	end

    // State flip-flops (sequential)
    always @(posedge clk) begin
        if(reset)
            state <= START;
    	else
            state <= next_state;
	end
 
    // Output logic
    assign done = (state == BYTE3);
endmodule
