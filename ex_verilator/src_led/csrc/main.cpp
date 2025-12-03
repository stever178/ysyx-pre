#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <nvboard.h>
#include "verilated.h"
#include "Vlight.h"

static TOP_NAME top;

void nvboard_bind_all_pins(TOP_NAME*);

void single_cycle() {
  top.clk = 0; top.eval();
  top.clk = 1; top.eval();
}

void reset(int n) {
  top.rst = 1;
  while (n -- > 0) single_cycle();
  top.rst = 0;
}

int main(int argc, char **argv) {
  // VerilatedContext* contextp = new VerilatedContext;
  auto contextp = std::make_unique<VerilatedContext>();

  // contextp->debug(0);
  contextp->traceEverOn(true);
  contextp->commandArgs(argc, argv);

  nvboard_bind_all_pins(&top);
  nvboard_init();

  reset(10); 
  while (!contextp->gotFinish()) {
    nvboard_update();

    single_cycle();

    // contextp->timeInc(1); // 1 timeprecision period passes...

    // printf("a = %d, b = %d, f = %d\n ", a, b, top->f);
    // VL_PRINTF("a = %d, b = %d, f = %d\n ", a, b, top->f);
    // assert(top->f == (a ^ b));
  }

  nvboard_quit();
  // delete top;
  // delete contextp;

  return 0;
}
