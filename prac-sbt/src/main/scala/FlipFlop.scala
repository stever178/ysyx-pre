import chisel3._
import circt.stage.ChiselStage

class FlipFlop extends Module {
  val io = IO(new Bundle {
    val d = Input(Bool())
    val q = Output(Bool())
  })

  val reg = RegInit(false.B)
  reg := io.d
  io.q := reg
}

object FlipFlop extends App {
  println(ChiselStage.emitSystemVerilog(new FlipFlop()))
}

