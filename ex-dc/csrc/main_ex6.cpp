#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Vex6.h"
#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define MAX_SIM_TIME 1e4

void nvboard_bind_all_pins(TOP_NAME *top);

/*
module light_seg_x (
    input [3:0] digit,
    output [6:0] seg
);
    MuxKey #(16, 4, 7) digit2hex (seg, digit, {
        4'h0, 7'b000_0001,
        4'h1, 7'b100_1111,
        4'h2, 7'b001_0010,
        4'h3, 7'b000_0110,

        4'h4, 7'b100_1100,
        4'h5, 7'b010_0100,
        4'h6, 7'b010_0000,
        4'h7, 7'b000_1101,

        4'h8, 7'b000_0000,
        4'h9, 7'b000_0100,
        4'ha, 7'b000_1000,
        4'hb, 7'b110_0000,

        4'hc, 7'b011_0001,
        4'hd, 7'b100_0010,
        4'he, 7'b011_0000,
        4'hf, 7'b011_1000
    });
endmodule
*/

// 7段数码管显示码表（对应0-15的十六进制数字）
static const uint8_t seg_table[16] = {
    0x01, // 0: 7'b000_0001
    0x4F, // 1: 7'b100_1111
    0x12, // 2: 7'b001_0010
    0x06, // 3: 7'b000_0110
    0x4C, // 4: 7'b100_1100
    0x24, // 5: 7'b010_0100
    0x20, // 6: 7'b010_0000
    0x0D, // 7: 7'b000_1101
    0x00, // 8: 7'b000_0000
    0x04, // 9: 7'b000_0100
    0x08, // A: 7'b000_1000
    0x60, // B: 7'b110_0000
    0x31, // C: 7'b011_0001
    0x42, // D: 7'b100_0010
    0x30, // E: 7'b011_0000
    0x38  // F: 7'b011_1000
};

static void single_cycle(TOP_NAME *dut) {
  dut->clk = 0;
  dut->eval();
  dut->clk = 1;
  dut->eval();
}

static void reset(TOP_NAME *dut, int n) {
  dut->reset = 1;
  while (n-- > 0)
    single_cycle(dut);
  dut->reset = 0;
}

int main(int argc, char **argv) {
  srand(time(NULL));
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  TOP_NAME *dut = new TOP_NAME();

  /* todo */
  bool flag_use_board = false;
  // flag_use_board = true;
  if (flag_use_board) {
    /* board */
    nvboard_bind_all_pins(dut);
    nvboard_init();

    while (1) {
      nvboard_update();
      dut->eval();
      printf("cycle\n");
    }

    nvboard_quit();
    delete dut;
    return 0;
  }

  /* C++ wave */
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("./waveform.vcd");

  /* verification */
  vluint64_t sim_time = 0;
  uint8_t state = 0;
  uint8_t seg0 = 0, seg1 = 0;
  uint8_t digit0 = 0, digit1 = 0;

  //
  dut->clk = 0;
  state = 0x01;
  dut->data_in = state;
  reset(dut, 1);
  printf("[%04lu] state: 0x%02x ; dut: 0x%02x\n", sim_time, state,
         dut->data_out);

  while (sim_time < MAX_SIM_TIME) {
    single_cycle(dut);

    if (dut->clk) {
      // ref model
      // next_state = { ^{state[4:2], state[0]}, state[7:1]};
      uint8_t xor_result = ((state >> 4) & 1) ^ ((state >> 3) & 1) ^
                           ((state >> 2) & 1) ^ (state & 1);
      state = (xor_result << 7) | ((state >> 1) & 0x7F);

      digit0 = state & 0x0F;
      seg0 = seg_table[digit0];

      digit1 = (state >> 4) & 0x0F;
      seg1 = seg_table[digit1];

      // assert
      printf("[%04lu] state: 0x%02x ; dut: 0x%02x\n", sim_time, state,
             dut->data_out);
      assert(dut->seg1 == seg1);
      assert(dut->seg0 == seg0);
    }

    // m_trace->dump(sim_time);
    sim_time++;
  }

  m_trace->close();
  delete dut;

  return 0;
}
