// https://hdlbits.01xz.net/wiki/Count_clock

module top_module(
    input clk,
    input reset,
    input ena,
    output pm,
    output [7:0] hh,
    output [7:0] mm,
    output [7:0] ss); 
    
    reg pm_reg;
    always @(posedge clk) begin
        if (reset)
            pm_reg <= 1'b0;
        else if (hh == 8'h11 & ena_hh)
            pm_reg <= ~pm_reg;
    end
    assign pm = pm_reg;
    
    wire ena_hh, ena_mm, ena_ss;
    
    assign ena_ss = ena;
    assign ena_mm = ss == 8'h59 & ena_ss;
    assign ena_hh = mm == 8'h59 & ena_mm;
    
    count_to_num(clk, reset, ena_hh, 8'h12, 8'h01, 8'h12, hh);    
    count_to_num(clk, reset, ena_mm, 8'h00, 8'h00, 8'h59, mm);
    count_to_num(clk, reset, ena_ss, 8'h00, 8'h00, 8'h59, ss);
endmodule

module count_to_num(
    input clk,
    input reset,
    input ena,
    input [7:0] num_reset,
    input [7:0] num_start,
    input [7:0] num_target,
    output [7:0] out);
    
    always @(posedge clk) begin
        if (reset)
            out <= num_reset;
        else if (ena) begin
            if (out[3:0] == 4'd9) 
                out[3:0] <= 4'd0;
            else
                out[3:0] <= out[3:0] + 8'd1;
            
            if (out == num_target)
                out[7:0] <= num_start;
            else if (out[3:0] == 4'd9)
               out[7:4] <= out[7:4] + 1'd1;
        end
    end    
endmodule
