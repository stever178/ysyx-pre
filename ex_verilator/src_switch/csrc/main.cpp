#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vswitch.h"

#define MAX_SIM_TIME 1e6

void nvboard_bind_all_pins(TOP_NAME* top);

struct VswitchDeleter {
  void operator()(Vswitch *p) {
    // 如果 Verilator 对象需要特殊清理：
    // p->final();  // 如果需要调用 final
    delete p;
  }
};

int main(int argc, char **argv) {
  /* RTL dumpfile */
  Verilated::traceEverOn(true);
  // Verilated::mkdir("logs");

  // auto contextp = std::make_unique<VerilatedContext>();
  // contextp->debug(0);
  // contextp->commandArgs(argc, argv);
  // contextp->traceEverOn(true);

  // auto dut = std::unique_ptr<Vswitch, VswitchDeleter>(new Vswitch{
    // contextp.get()
  // });

  TOP_NAME* dut = new TOP_NAME();
  // auto dut = top;

  /* C++ wave */
  VerilatedVcdC *m_trace = new VerilatedVcdC;
  dut->trace(m_trace, 5);
  m_trace->open("waveform.vcd");


  /* board */
  // nvboard_bind_all_pins(dut);
  // nvboard_init();

  int8_t a = rand() & 1;
  int8_t b = rand() & 1;
  // while (!contextp->gotFinish()) {

  vluint64_t sim_time = 0;
  while (sim_time <= MAX_SIM_TIME) {
    // nvboard_update();

    a = !a;
    b = !b;
    dut->a = a;
    dut->b = b;

    dut->eval();
    m_trace->dump(sim_time);

    printf("time: %2zd, a = %2d, b = %2d, f = %2d\n ",
           sim_time, a, b, dut->f);
    // VL_PRINTF("a = %d, b = %d, f = %d\n ", a, b, top->f);
    // assert(top->f == (a ^ b));
    
    // contextp->timeInc(1); // 1 timeprecision period passes...
    sim_time++;
  }

  // nvboard_quit();

  m_trace->close();

  // delete dut;
  // delete contextp;

  return 0;
}
