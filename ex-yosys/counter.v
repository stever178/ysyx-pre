// counter.v
module counter(
  input clk,
  input rst,
  input en,
  output reg [1:0] count
);
  always @(posedge clk) begin
    if (rst) count <= 2'd0;
    else if (en) count <= count + 2'd1;
  end
endmodule

