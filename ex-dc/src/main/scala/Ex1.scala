// src/main/scala/Ex1.scala
package ex1

import chisel3._
import chisel3.util._
import chisel3.stage._
import chisel3.emitVerilog

class Ex1 extends RawModule {
// class Ex1 extends Module {
  val io = IO(new Bundle {
    val X0 = Input(UInt(2.W))
    val X1 = Input(UInt(2.W))
    val X2 = Input(UInt(2.W))
    val X3 = Input(UInt(2.W))
    val Y  = Input(UInt(2.W))

    val F = Output(UInt(2.W))
  })

  io.F := MuxLookup(io.Y, 0x0.U(2.W))(Seq(
    0b00.U -> io.X0,
    0b01.U -> io.X1,
    0b10.U -> io.X2,
    0b11.U -> io.X3
  ))
}

object Ex1Elaborate extends App {
  val firtoolOptions = Array(
    "--lowering-options=" + List(
      // make yosys happy
      // see https://github.com/llvm/circt/blob/main/docs/VerilogGeneration.md
      "disallowLocalVariables",
      "disallowPackedArrays",
      "locationInfoStyle=wrapInAtSquareBracket"
    ).reduce(_ + "," + _)
  )
  _root_.circt.stage.ChiselStage.emitSystemVerilogFile(new Ex1(), args, firtoolOptions)
}
