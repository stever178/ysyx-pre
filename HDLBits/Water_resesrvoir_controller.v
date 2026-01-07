// https://hdlbits.01xz.net/wiki/Exams/ece241_2013_q4

module top_module (
    input clk,
    input reset,
    input [3:1] s,
    output fr3,
    output fr2,
    output fr1,
    output dfr
); 
    parameter [2:0] state1 = 3'b000;
    parameter [2:0] state2 = 3'b001;
    parameter [2:0] state3 = 3'b011;
    parameter [2:0] state4 = 3'b111;
    
    reg [2:0] state, next_state;
    reg [2:0] outfr;
    
    assign {fr3,fr2,fr1} = outfr;

    assign next_state = s;
    
    always @(posedge clk) begin
        if (reset)
            state <= state1;
        else
            state <= next_state;
    end

    always @(posedge clk) begin
        if (reset) begin
            outfr <= 3'b111;
        end else begin        
            case (next_state)
                state4: outfr <= 3'b000;
                state3: outfr <= 3'b001;
                state2: outfr <= 3'b011;
                state1: outfr <= 3'b111;
            endcase
        end
    end
    
    always @(posedge clk) begin
        if (reset) begin
            dfr <= 1'b1;
        end else begin        
            if (state > next_state)
				dfr <= 1'b1;
            else if (state < next_state)
                dfr <= 1'b0;
            else begin
                dfr <= dfr;
            end
        end
    end
endmodule
