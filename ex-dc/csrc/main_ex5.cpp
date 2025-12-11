#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Vex5.h"
#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

#define MAX_SIM_TIME 1e6

void nvboard_bind_all_pins(TOP_NAME *top);

int main(int argc, char **argv) {
  srand(time(NULL));
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  TOP_NAME *dut = new TOP_NAME();

  /* C++ wave */
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");

  /* board */
  nvboard_bind_all_pins(dut);
  nvboard_init();

  while (1) { 
    nvboard_update();
    dut->eval();
  }
  
  vluint64_t sim_time = 0;
  for (const auto &test_pair : data_arr) {
    // {A, B, sel, out_o, out_c, out_z, out_s}
    printf("#%3ld expected: ", sim_time);
    print_test_case(test_pair);

    dut->A = test_pair.A;
    dut->B = test_pair.B;
    dut->sel = test_pair.sel;

    dut->eval();
    
    assert(dut->out_o == test_pair.out_o);
    assert(dut->out_c == test_pair.out_c);

    assert(dut->out_s == test_pair.out_s);
    assert(dut->out_z == test_pair.out_z);

    // m_trace->dump(sim_time);
    sim_time++;
  }

  nvboard_quit();
  m_trace->close();
  delete dut;

  return 0;
}
