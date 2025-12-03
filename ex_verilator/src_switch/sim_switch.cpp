#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <nvboard.h>
#include "Vswitch.h"
#include "verilated.h"

static TOP_NAME dut;

void nvboard_bind_all_pins(TOP_NAME* top);

struct VswitchDeleter {
  void operator()(Vswitch *p) {
    // 如果 Verilator 对象需要特殊清理：
    // p->final();  // 如果需要调用 final
    delete p;
  }
};

int main(int argc, char **argv) {
  Verilated::mkdir("logs");

  auto contextp = std::make_unique<VerilatedContext>();

  // contextp->debug(0);
  contextp->traceEverOn(true);
  contextp->commandArgs(argc, argv);

  auto top =
      std::unique_ptr<Vswitch, VswitchDeleter>(new Vswitch{contextp.get()});

  nvboard_bind_all_pins(&dut);
  nvboard_init();

  while (!contextp->gotFinish()) {
    nvboard_update();

    // int a = rand() & 1;
    // int b = rand() & 1;
    // top->a = a;
    // top->b = b;

    // contextp->timeInc(1); // 1 timeprecision period passes...
    // top->eval();
    dut.eval();

    // printf("a = %d, b = %d, f = %d\n ", a, b, top->f);
    // VL_PRINTF("a = %d, b = %d, f = %d\n ", a, b, top->f);
    // assert(top->f == (a ^ b));
  }

  nvboard_quit();

  // delete top;
  // delete contextp;

  return 0;
}
