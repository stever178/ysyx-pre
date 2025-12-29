// https://hdlbits.01xz.net/wiki/Dualedge

module top_module (
    input clk,
    input d,
    output q
);
    reg q1, q2;
        
    always @(posedge clk) begin
        q1 <= d ^ q2; 
    end
    always @(negedge clk) begin
        q2 <= d ^ q1; 
    end

    assign q = q1 ^ q2;
endmodule
