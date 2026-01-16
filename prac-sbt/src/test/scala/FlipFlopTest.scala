import chisel3._
import org.scalatest.flatspec.AnyFlatSpec

class FlipFlopTest extends AnyFlatSpec with ChiselScalatestTester {
  "FlipFlop" should "correctly store the input on the rising edge of the clock" in {
    test(new FlipFlop) { c =>
      // 初始状态
      c.io.d.poke(false.B)   // 设置输入 d
      c.clock.step(1)        // 推进时钟
      c.io.q.expect(false.B) // 检查输出 q

      // 设置 D 为 true
      c.io.d.poke(true.B)
      c.clock.step(1)
      c.io.q.expect(true.B)

      // 设置 D 为 false
      c.io.d.poke(false.B)
      c.clock.step(1)
      c.io.q.expect(false.B)
    }
  }
}

