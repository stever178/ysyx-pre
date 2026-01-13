#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vex6_barrel_shifter.h"
#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

void nvboard_bind_all_pins(TOP_NAME *top);

#define MAX_SIM_TIME 1e4

/*
module ex6_barrel_shifter(
        input LRw,
        input ALw,
        input [2:0] shamt,
    input [7:0] data_in,
    output [7:0] data_out
);
*/
struct DUT_INPUT {
  uint8_t LRw, ALw, shamt;
  uint8_t data_in;
} data_arr[] = {
    {1, 0, 1, 0x01}, {1, 1, 1, 0x03}, {1, 0, 3, 0x05}, {1, 1, 3, 0x09},
    {1, 0, 2, 0x0a}, {1, 1, 2, 0x11}, {1, 0, 4, 0x31}, {1, 1, 7, 0x21},

    {0, 0, 1, 0x01}, {0, 1, 1, 0x03}, {0, 0, 3, 0x05}, {0, 1, 3, 0x09},
    {0, 0, 2, 0x0a}, {0, 1, 2, 0x11}, {0, 0, 4, 0x31}, {0, 1, 7, 0x21},
};

int main(int argc, char **argv) {
  srand(time(NULL));
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  TOP_NAME *dut = new TOP_NAME();

  /* C++ wave */
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");

  /* verification */

  bool use_board = argc > 1 && strcmp(argv[1], "--use_board") == 0;
  if (use_board) {
    /* board */
    nvboard_bind_all_pins(dut);
    nvboard_init();

    for (;;) {
      nvboard_update();
      dut->eval();
    }
    nvboard_quit();
    m_trace->close();
    delete dut;
    return 0;
  }

  vluint64_t sim_time = 0;
  for (const auto &test_pair : data_arr) {
    dut->LRw = test_pair.LRw;
    dut->ALw = test_pair.ALw;
    dut->shamt = test_pair.shamt;
    dut->data_in = test_pair.data_in;

    dut->eval();

    printf(
        "[%2lu] LRw: %d, ALw: %d, shamt: %d, data_in: %02x, data_out: %02x, ",
        sim_time, test_pair.LRw, test_pair.ALw, test_pair.shamt,
        test_pair.data_in, dut->data_out);

    uint8_t data_out = 0;
    if (test_pair.LRw == 1) {
      data_out = test_pair.data_in << test_pair.shamt;
    } else {
      if (test_pair.ALw == 1) {
        // 算术右移 - 需要是有符号类型
        data_out = (int8_t)test_pair.data_in >> test_pair.shamt;
      } else {
        // 逻辑右移 - 转换为无符号类型
        data_out = (uint8_t)test_pair.data_in >> test_pair.shamt;
      }
    }
    printf("data_out_expected: %02x\n", data_out);
    assert((uint8_t)dut->data_out == data_out);

    // m_trace->dump(sim_time);
    sim_time++;
  }
  m_trace->close();

  delete dut;
  return 0;
}
