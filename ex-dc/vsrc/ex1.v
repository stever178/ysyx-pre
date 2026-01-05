// https://nju-projectn.github.io/dlco-lecture-note/exp/01.html#id8

`include "mux_template.v"

module ex1(
  input  [1:0] X0, X1, X2, X3,
  input  [1:0] Y,
  output [1:0] F
);
  Mux4to1 top (
    X0, X1, X2, X3, Y, F
  );
endmodule

module Mux4to1(
  input  [1:0] X0, X1, X2, X3,
  input  [1:0] Y,
  output [1:0] F
);
  MuxKey #(4, 2, 2) i0 (F, Y, {
    2'b00, X0,
    2'b01, X1,
    2'b10, X2,
    2'b11, X3
  });
endmodule
