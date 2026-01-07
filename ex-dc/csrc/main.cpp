#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vex1.h"

#define MAX_SIM_TIME 1e6

void nvboard_bind_all_pins(TOP_NAME* top);

int main(int argc, char **argv) {
  srand (time(NULL));
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);

  /* RTL dumpfile */
  // Verilated::mkdir("logs");

  // auto contextp = std::make_unique<VerilatedContext>();
  // contextp->debug(0);
  // contextp->commandArgs(argc, argv);
  // contextp->traceEverOn(true);

  // auto dut = std::unique_ptr<Vswitch, VswitchDeleter>(new Vswitch{
  //   contextp.get()
  // });

  TOP_NAME* dut = new TOP_NAME();

  /* C++ wave */
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");

  /* board */
  nvboard_bind_all_pins(dut);
  nvboard_init();

  // int8_t X0, X1, X2, X3, Y;
  
  vluint64_t sim_time = 0;
  // while (!contextp->gotFinish()) {
  // while (sim_time <= MAX_SIM_TIME) {
  while (1) {
    nvboard_update();

    // dut->X0 = X0;
    // dut->X1 = X1;

    dut->eval();
    
    // m_trace->dump(sim_time);
    // printf("time: %2zd, X0 = %2d, X1 = %2d, X2 = %2d, X3 = %2d, F = %2d\n ",
    //        sim_time, X0, X1, X2, X3, dut->F);
    // VL_PRINTF("a = %d, b = %d, f = %d\n ", a, b, top->f);
    // assert(top->f == (a ^ b));
    
    // contextp->timeInc(1); // 1 timeprecision period passes...
    sim_time++;
  }

  nvboard_quit();

  m_trace->close();

  // delete dut;
  // delete contextp;

  return 0;
}
