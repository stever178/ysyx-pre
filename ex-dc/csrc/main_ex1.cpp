#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vex1.h"
#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

void nvboard_bind_all_pins(TOP_NAME *top);

#define MAX_SIM_TIME 1e4

/*
二位四选一选择器
module ex1(
  input  [1:0] X0, X1, X2, X3,
  input  [1:0] Y,
  output [1:0] F
);
*/
struct DUT_INPUT {
  uint8_t X[4], Y;
} data_arr[] = {
    {{0x00, 0x01, 0x02, 0x03}, 0x00},
    {{0x00, 0x01, 0x02, 0x03}, 0x01},
    {{0x00, 0x01, 0x02, 0x03}, 0x02},
    {{0x00, 0x01, 0x02, 0x03}, 0x03},
};

struct DUT_OUTPUT {
  uint8_t F;
} ref_model;

void assign_dut(TOP_NAME *dut, const DUT_INPUT *ref_in) {
  dut->X0 = ref_in->X[0];
  dut->X1 = ref_in->X[1];
  dut->X2 = ref_in->X[2];
  dut->X3 = ref_in->X[3];
  dut->Y = ref_in->Y;
}

void eval_ref(const DUT_INPUT *ref_in, DUT_OUTPUT *ref_out) {
  ref_out->F = ref_in->X[ref_in->Y];
}

void print_data(const TOP_NAME *dut, const DUT_OUTPUT *ref_out,
                vluint64_t sim_time) {
  printf("cycle[%lu]\n", sim_time);

  printf("X0: %02b, X1: %02b, X2: %02b, X3: %02b, Y: %02b\n", dut->X0, dut->X1,
         dut->X2, dut->X3, dut->Y);
  printf("\tdut_F: %02b\n", dut->F);
  printf("\tref_F: %02b\n", ref_out->F);
}

void assert_ref(const TOP_NAME *dut, const DUT_OUTPUT *ref_out) {
  assert(dut->F == ref_out->F);
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

    for (;;) {
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
