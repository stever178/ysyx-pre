// https://hdlbits.01xz.net/wiki/Exams/ece241_2013_q12

module top_module (
    input clk,
    input enable,
    input S,
    input A, B, C,
    output Z ); 
    
    reg [7:0] Q;
    
    always @(posedge clk) begin
        if (enable)
            Q[7:0] <= {Q[6:0], S};
        else
            Q[7:0] <= Q[7:0];
    end

    assign Z = Q[{A, B, C}];
endmodule
