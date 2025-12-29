// https://hdlbits.01xz.net/wiki/Fsm3comb

module top_module(
    input in,
    input [1:0] state,
    output [1:0] next_state,
    output out); //

    parameter A=0, B=1, C=2, D=3;

    // State transition logic: next_state = f(state, in)
    always @(*) begin
        // State transition logic
        next_state = A;
        
        case (state)
            A: begin
                if (in == 1'b0)
                    next_state = A;
                else if (in == 1'b1)
                    next_state = B;
            end
            B: begin
                if (in == 1'b0)
                    next_state = C;
                else if (in == 1'b1)
                    next_state = B;
            end
            C: begin
                if (in == 1'b0)
                    next_state = A;
                else if (in == 1'b1)
                    next_state = D;
            end
            D: begin
                if (in == 1'b0)
                    next_state = C;
                else if (in == 1'b1)
                    next_state = B;
            end
        endcase
    end

    // Output logic:  out = f(state) for a Moore state machine
    assign out = (state == D);
    
endmodule
