// https://nju-projectn.github.io/dlco-lecture-note/exp/02.html#id9

module ex2(
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
  integer i;
  always @(din or en) begin
    if (en) begin
      dout = 3'b000;
      for(i = 0; i <= 7; i = i + 1)
        if(din[i] == 1)  dout = i[2:0];
    end
    else  dout = 3'b000;
  end
  assign valid = |din;
endmodule

module encode83(
  input  [7:0] x,
  input  en,
  output reg [2:0] y
);
  always @(x or en) begin
    if (en) begin
      case (x)
          8'b0000_0001 : y = 3'b000;
          8'b0000_0010 : y = 3'b001;
          8'b0000_0100 : y = 3'b010;
          8'b0000_1000 : y = 3'b011;
          8'b0001_0000 : y = 3'b100;
          8'b0010_0000 : y = 3'b101;
          8'b0100_0000 : y = 3'b110;
          8'b1000_0000 : y = 3'b111;
          default: y = 3'b000;
      endcase
    end
    else  y = 3'b000;
  end
endmodule
