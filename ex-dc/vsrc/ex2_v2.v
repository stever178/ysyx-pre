`include "mux_template.v"

module ex2_v2(
  input  [7:0] x,
  input  EN,
  output reg [2:0] y,
  output reg [6:0] seg,
  output valid
);
  prio_encode83 encoder (
    .din(x), .en(EN), .dout(y), .valid(valid)
  );
  light_seg seg0 (
    .x(y), .seg(seg)
  );
endmodule

module light_seg(
  input [2:0] x,
  output reg [6:0] seg
);
  always @(x) begin
    case (x)
      3'b000 : seg = 7'b000_0001;
      3'b001 : seg = 7'b100_1111;
      3'b010 : seg = 7'b001_0010;
      3'b011 : seg = 7'b000_0110;
      3'b100 : seg = 7'b100_1100;
      3'b101 : seg = 7'b010_0100;
      3'b110 : seg = 7'b010_0000;
      3'b111 : seg = 7'b000_1101;
      default : seg = 7'b111_1111;
    endcase
  end
endmodule

module prio_encode83(
  input  [7:0] din,
  input  en,
  output reg [2:0] dout,
  output valid
);
  wire [7:0] sel_onehot;
  assign sel_onehot[7] = din[7];
  assign sel_onehot[6] = din[6] & (~din[7]);
  assign sel_onehot[5] = din[5] & (~din[7] & ~din[6]);
  assign sel_onehot[4] = din[4] & (~din[7] & ~din[6] & ~din[5]);
  assign sel_onehot[3] = din[3] & (~din[7] & ~din[6] & ~din[5] & ~din[4]);
  assign sel_onehot[2] = din[2] & (~din[7] & ~din[6] & ~din[5] & ~din[4] & ~din[3]);
  assign sel_onehot[1] = din[1] & (~din[7] & ~din[6] & ~din[5] & ~din[4] & ~din[3] & ~din[2]);
  assign sel_onehot[0] = din[0] & (~din[7] & ~din[6] & ~din[5] & ~din[4] & ~din[3] & ~din[2] & ~din[1]);

  wire [7:0] sel;
  assign sel = sel_onehot & {8{en}};

  MuxKey #(8, 8, 3) mux_8to3 (dout, sel, {
    8'b0000_0001, 3'b000,
    8'b0000_0010, 3'b001,
    8'b0000_0100, 3'b010,
    8'b0000_1000, 3'b011,
    
    8'b0001_0000, 3'b100,
    8'b0010_0000, 3'b101,
    8'b0100_0000, 3'b110,
    8'b1000_0000, 3'b111
  });

  assign valid = |din;
endmodule
