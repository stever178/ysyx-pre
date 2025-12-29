#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vex2.h"

#define MAX_SIM_TIME 1e6

void nvboard_bind_all_pins(TOP_NAME* top);

int main(int argc, char **argv) {
  srand (time(NULL));
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  TOP_NAME* dut = new TOP_NAME();

  /* C++ wave */
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");

  /* board */
  nvboard_bind_all_pins(dut);
  nvboard_init();

  vluint64_t sim_time = 0;
  while (1) {
    nvboard_update();
    dut->eval();
    
    sim_time++;
  }
  nvboard_quit();

  m_trace->close();
  delete dut;

  return 0;
}
