// src/main/scala/Ex2.scala
package ex2

import chisel3._
import chisel3.util._
import chisel3.stage._
import chisel3.emitVerilog

/**
 * 顶层模块：8位优先级编码器 + 7段数码管显示
 * 对应Verilog的ex2_v2模块
 */
class Ex2 extends RawModule {
  // 定义IO端口（和Verilog完全一致）
  val io = IO(new Bundle {
    val x     = Input(UInt(8.W))    // 8位输入
    val EN    = Input(Bool())       // 使能
    val y     = Output(UInt(3.W))   // 3位编码输出
    val seg   = Output(UInt(7.W))   // 7段数码管输出
    val valid = Output(Bool())      // 有效标志
  })

  // 例化优先级编码器模块（对应prio_encode83）
  val encoder = Module(new PrioEncode83)
  encoder.io.din   := io.x
  encoder.io.en    := io.EN
  io.y             := encoder.io.dout
  io.valid         := encoder.io.valid

  // 例化7段数码管模块（对应light_seg）
  val segModule = Module(new LightSeg)
  segModule.io.x   := io.y
  io.seg           := segModule.io.seg
}

/**
 * 7段数码管显示模块
 * 对应Verilog的light_seg模块
 */
class LightSeg extends RawModule {
  val io = IO(new Bundle {
    val x   = Input(UInt(3.W))    // 3位输入（优先级编码器输出）
    val seg = Output(UInt(7.W))   // 7段数码管输出
  })

  // 用Chisel的MuxLookup替代Verilog的MuxKey
  // 映射关系和Verilog完全一致（注意：Chisel的UInt是小端序，但数值等价）
  io.seg := MuxLookup(io.x, 0b0000001.U(7.W))(Seq(
    0.U(3.W) -> 0b0000001.U(7.W),  // 0: 000_0001
    1.U(3.W) -> 0b1001111.U(7.W),  // 1: 100_1111
    2.U(3.W) -> 0b0010010.U(7.W),  // 2: 001_0010
    3.U(3.W) -> 0b0000110.U(7.W),  // 3: 000_0110
    4.U(3.W) -> 0b1001100.U(7.W),  // 4: 100_1100
    5.U(3.W) -> 0b0100100.U(7.W),  // 5: 010_0100
    6.U(3.W) -> 0b0100000.U(7.W),  // 6: 010_0000
    7.U(3.W) -> 0b0001101.U(7.W)   // 7: 000_1101
  ))
}

/**
 * 8-3优先级编码器模块
 * 对应Verilog的prio_encode83模块
 */
class PrioEncode83 extends RawModule {
  val io = IO(new Bundle {
    val din   = Input(UInt(8.W))   // 8位输入
    val en    = Input(Bool())      // 使能
    val dout  = Output(UInt(3.W))  // 3位编码输出
    val valid = Output(Bool())     // 有效标志
  })

  // 步骤1：生成onehot选择信号（sel_onehot）
  val bit7 = io.din(7)
  val bit6 = io.din(6) & (~io.din(7))
  val bit5 = io.din(5) & (~io.din(7) & ~io.din(6))
  val bit4 = io.din(4) & (~io.din(7) & ~io.din(6) & ~io.din(5))
  val bit3 = io.din(3) & (~io.din(7) & ~io.din(6) & ~io.din(5) & ~io.din(4))
  val bit2 = io.din(2) & (~io.din(7) & ~io.din(6) & ~io.din(5) & ~io.din(4) & ~io.din(3))
  val bit1 = io.din(1) & (~io.din(7) & ~io.din(6) & ~io.din(5) & ~io.din(4) & ~io.din(3) & ~io.din(2))
  val bit0 = io.din(0) & (~io.din(7) & ~io.din(6) & ~io.din(5) & ~io.din(4) & ~io.din(3) & ~io.din(2) & ~io.din(1))
  val selOneHot = Cat(bit7, bit6, bit5, bit4, bit3, bit2, bit1, bit0)  // 8位，bit7是最高位
  // val selOneHot = WireInit(0.U(8.W))
  // selOneHot(7) := io.din(7)
  // selOneHot(6) := io.din(6) & (~io.din(7))
  // selOneHot(5) := io.din(5) & (~io.din(7) & ~io.din(6))
  // selOneHot(4) := io.din(4) & (~io.din(7) & ~io.din(6) & ~io.din(5))
  // selOneHot(3) := io.din(3) & (~io.din(7) & ~io.din(6) & ~io.din(5) & ~io.din(4))
  // selOneHot(2) := io.din(2) & (~io.din(7) & ~io.din(6) & ~io.din(5) & ~io.din(4) & ~io.din(3))
  // selOneHot(1) := io.din(1) & (~io.din(7) & ~io.din(6) & ~io.din(5) & ~io.din(4) & ~io.din(3) & ~io.din(2))
  // selOneHot(0) := io.din(0) & (~io.din(7) & ~io.din(6) & ~io.din(5) & ~io.din(4) & ~io.din(3) & ~io.din(2) & ~io.din(1))

  // 步骤2：使能信号掩码（对应Verilog的{8{en}}）
  val sel = Wire(UInt(8.W))
  sel := selOneHot & Fill(8, io.en)  // Fill(8, en) 等价于Verilog的{8{en}}

  // 步骤3：onehot到3位编码的映射（替代Verilog的MuxKey）
  io.dout := MuxLookup(sel, 0b000.U(3.W))(Seq(
    0b00000001.U(8.W) -> 0b000.U(3.W),  // 最低位有效
    0b00000010.U(8.W) -> 0b001.U(3.W),
    0b00000100.U(8.W) -> 0b010.U(3.W),
    0b00001000.U(8.W) -> 0b011.U(3.W),
    0b00010000.U(8.W) -> 0b100.U(3.W),
    0b00100000.U(8.W) -> 0b101.U(3.W),
    0b01000000.U(8.W) -> 0b110.U(3.W),
    0b10000000.U(8.W) -> 0b111.U(3.W)   // 最高位有效
  ))

  // 步骤4：有效标志（对应Verilog的|din）
  io.valid := io.din.orR  // orR等价于Verilog的|din（按位或）
}

object Ex2Elaborate extends App {
  val firtoolOptions = Array(
    "--lowering-options=" + List(
      // make yosys happy
      // see https://github.com/llvm/circt/blob/main/docs/VerilogGeneration.md
      "disallowLocalVariables",
      "disallowPackedArrays",
      "locationInfoStyle=wrapInAtSquareBracket"
    ).reduce(_ + "," + _)
  )
  _root_.circt.stage.ChiselStage.emitSystemVerilogFile(new Ex2(), args, firtoolOptions)
}
