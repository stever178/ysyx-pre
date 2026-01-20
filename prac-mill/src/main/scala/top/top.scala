// src/main/scala/top.scala
package top

import chisel3._
import chisel3.util._
import chisel3.emitVerilog
// wrong!
// import chisel3.stage.ChiselStage

import decoder._

class Top extends Module {
  val io = IO(new Bundle {
    val in = Input(UInt(3.W))
    val out = Output(UInt(8.W))
  })

  val decoder = Module(new Decoder)
  decoder.io.in := io.in
  io.out := decoder.io.out
}

object Top extends App {
  emitVerilog(new Top, args)
  //emitVerilog(new Top)
}

// object Top {
//   def main(args: Array[String]): Unit = {
//     // 1. 生成 Verilog 字符串并打印到控制台
//     val verilogCode = emitVerilog(new Top, args)
//     println("=== 生成的 Verilog 代码 ===")
//     println(verilogCode)
// 
//     // 2. 同时将 Verilog 写入文件（可选，避免只打印不落地）
//     import java.nio.file.{Paths, Files}
//     Files.write(Paths.get("Top.v"), verilogCode.getBytes)
//     println("\n=== Verilog 已写入 Top.v 文件 ===")
// 
//     // 3. 主动退出 JVM，解决程序不终止问题
//     // sys.exit(0)
//   }
// }

