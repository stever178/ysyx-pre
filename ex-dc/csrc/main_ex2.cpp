#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vex2.h"
#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

void nvboard_bind_all_pins(TOP_NAME *top);

#define MAX_SIM_TIME 1e4

/*
优先编码器和数码管显示代码
module ex2(
  input  [7:0] x,
  input  EN,
  output [2:0] y,
  output [6:0] seg,
  output valid
);
*/
struct DUT_INPUT {
  uint8_t x;
  bool EN;
} data_arr[] = {
    {0b00000000, true},  {0b00000001, true},  {0b00010001, true},
    {0b01000001, true},  {0b00010001, true},  {0b00000101, true},
    {0b10010001, true},  {0b01000001, true},  {0b00101001, true},

    {0b00000000, false}, {0b00000001, false}, {0b00010001, false},
    {0b01000001, false}, {0b00010001, false}, {0b00000101, false},
    {0b10010001, false}, {0b01000001, false}, {0b00101001, false},
};

struct DUT_OUTPUT {
  uint8_t y;
  bool valid;
} ref_model;

void assign_dut(TOP_NAME *dut, const DUT_INPUT *ref_in) {
  dut->x = ref_in->x;
  dut->EN = ref_in->EN;
}

void eval_ref(const DUT_INPUT *ref_in, DUT_OUTPUT *ref_out) {
  ref_out->y = 0x00;
  ref_out->valid = (ref_in->x != 0x00);

  if (ref_in->EN == false) {
    return;
  }

  for (int8_t i = 7; i >= 0; i--) {
    if (ref_in->x & (1 << i)) {
      ref_out->y = i;
      break;
    }
  }
}

void assert_ref(const TOP_NAME *dut, const DUT_OUTPUT *ref_out) {
  assert(dut->y == ref_out->y);
  assert(dut->valid == ref_out->valid);
}

void print_data(const TOP_NAME *dut, const DUT_OUTPUT *ref_out,
                vluint64_t sim_time) {
  printf("cycle[%lu]\n", sim_time);
  printf("x: %08b, EN: %1b\n", dut->x, dut->EN);
  printf("\tdut_y: %03b, dut_valid: %1b\n", dut->y, dut->valid);
  printf("\tref_y: %03b, ref_valid: %1b\n", ref_out->y, ref_out->valid);
}

int main(int argc, char **argv) {
  srand(time(NULL));
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  TOP_NAME *dut = new TOP_NAME();

  vluint64_t sim_time = 0;

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

    while (1) {
      nvboard_update();
      dut->eval();
    }
    nvboard_quit();
    m_trace->close();
    delete dut;
    return 0;
  }

  assert(std::size(data_arr) > 1);
  for (const auto &test_pair : data_arr) {
    assign_dut(dut, &test_pair);

    dut->eval();
    eval_ref(&test_pair, &ref_model);

    print_data(dut, &ref_model, sim_time);
    assert_ref(dut, &ref_model);

    // m_trace->dump(sim_time);
    sim_time++;
  }
  m_trace->close();

  delete dut;
  return 0;
}
