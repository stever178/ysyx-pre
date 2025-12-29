// https://hdlbits.01xz.net/wiki/Fsm2

module top_module(
    input clk,
    input areset,    // Asynchronous reset to OFF
    input j,
    input k,
    output out); //  

    parameter OFF=1'b0, ON=1'b1; 
    reg state, next_state;

    always @(*) begin
        // State transition logic
        next_state = OFF;
        
        case (state)
            ON: begin
                if (k == 1'b0)
                    next_state = ON;
                else if (k == 1'b1)
                    next_state = OFF;
            end
            OFF: begin
                if (j == 1'b0)
                    next_state = OFF;
                else if (j == 1'b1)
                    next_state = ON;
            end
        endcase
    end

    always @(posedge clk, posedge areset) begin
        // State flip-flops with asynchronous reset
        if (areset)
            state <= OFF;
        else
            state <= next_state;
    end

    // Output logic
    assign out = (state == ON);

endmodule
