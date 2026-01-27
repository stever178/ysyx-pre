package ex2

import chisel3._
import chisel3.experimental.BundleLiterals._
import chisel3.simulator.scalatest.ChiselSim
import org.scalatest.freespec.AnyFreeSpec
import org.scalatest.matchers.must.Matchers

import chisel3.simulator.EphemeralSimulator._
import org.scalatest.flatspec.AnyFlatSpec

class Ex2Tester extends AnyFlatSpec {
  "Ex2 Module" should "be a 83 priority encoder" in {

// class Ex2Tester extends AnyFreeSpec with Matchers with ChiselSim {
//   "Ex2 Module should ..." in {

    simulate(new Ex2()) { dut =>
      val dataArr = Seq(
        DutInput(0x00, true),
        DutInput(0x01, true),
        DutInput(0x11, true),
        DutInput(0x41, true),
        DutInput(0x11, true),
        DutInput(0x05, true),
        DutInput(0x91, true),
        DutInput(0x41, true),
        DutInput(0x29, true),

        DutInput(0x00, false),
        DutInput(0x01, false),
        DutInput(0x11, false),
        DutInput(0x41, false),
        DutInput(0x11, false),
        DutInput(0x05, false),
        DutInput(0x91, false),
        DutInput(0x41, false),
        DutInput(0x29, false)
      )

      for ((in, cycle) <- dataArr.zipWithIndex) {
        // === assign_dut ===
        dut.io.x.poke(in.x.U)
        dut.io.EN.poke(in.en.B)

        // dut.clock.step()

        // === eval_ref ===
        val ref = evalRef(in)

        // === print ===
        println(s"cycle[$cycle]")

        val xBin = String.format("%8s", in.x.toBinaryString).replace(' ', '0')
        println(s"\tx=$xBin, EN=${in.en}")
        // println(f"\tx=${in.x}%8s, EN=${in.en}")

        println(s"\tdut_y=${dut.io.y.peek().litValue}, dut_valid=${dut.io.valid.peek().litToBoolean}")
        println(s"\tref_y=${ref.y}, ref_valid=${ref.valid}")

        // === assert_ref ===
        /* bad examples
        assert(dut.io.y.peek() == ref.y)
        assert(dut.io.valid.peek() == ref.valid)
         */
        assert(dut.io.y.peek().litValue == ref.y)
        assert(dut.io.valid.peek().litToBoolean == ref.valid)
      }
    }
  }

  // ---------- reference model ----------
  case class DutInput(x: Int, en: Boolean)
  case class DutOutput(y: Int, valid: Boolean)

  def evalRef(in: DutInput): DutOutput = {
    if (!in.en) {
      DutOutput(0, in.x != 0)
    } else {
      if (in.x == 0) {
        DutOutput(0, false)
      } else {
        val y = (7 to 0 by -1).find(i => ((in.x >> i) & 1) == 1).get
        DutOutput(y, true)
      }
    }
  }
}
