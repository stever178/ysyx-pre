package ex1

import chisel3._
import chisel3.experimental.BundleLiterals._
import chisel3.simulator.scalatest.ChiselSim
import org.scalatest.freespec.AnyFreeSpec
import org.scalatest.matchers.must.Matchers

import chisel3.simulator.EphemeralSimulator._
import org.scalatest.flatspec.AnyFlatSpec

class Ex1Tester extends AnyFlatSpec {
  "Ex1 Module" should "select correct input based on Y" in {

// class Ex1Tester extends AnyFreeSpec with Matchers with ChiselSim {
//   "Ex1 Module should select correct input based on Y" in {

    simulate(new Ex1()) { dut =>
      // 测试用例1：Y=00 → 输出X0
      dut.io.X0.poke(0b01.U)
      dut.io.X1.poke(0b10.U)
      dut.io.X2.poke(0b11.U)
      dut.io.X3.poke(0b00.U)
      dut.io.Y.poke(0b00.U)
      assert(dut.io.F.peek().litValue == 0b01) // 替代expect，更直观

      // 测试用例2：Y=01 → 输出X1
      dut.io.Y.poke(0b01.U)
      assert(dut.io.F.peek().litValue == 0b10)

      // 测试用例3：Y=10 → 输出X2
      dut.io.Y.poke(0b10.U)
      assert(dut.io.F.peek().litValue == 0b11)

      // 测试用例4：Y=11 → 输出X3
      dut.io.Y.poke(0b11.U)
      assert(dut.io.F.peek().litValue == 0b00)
    }
  }
}
