#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory>

#include "Vswitch.h"
#include "verilated.h"

int main(int argc, char** argv) {
    Verilated::mkdir("logs");
	
    //VerilatedContext* contextp = new VerilatedContext;
	auto contextp = std::make_shared<VerilatedContext>();

    //contextp->debug(0);
	contextp->traceEverOn(true);
    contextp->commandArgs(argc, argv);

    Vswitch *top = new Vswitch{contextp.get()};

    while (!contextp->gotFinish()) { 
		int a = rand() & 1;
		int b = rand() & 1;
		top->a = a;
		top->b = b;

        contextp->timeInc(1);  // 1 timeprecision period passes...
		top->eval(); 

		printf("a = %d, b = %d, f = %d\n ", a, b, top->f);
		//VL_PRINTF("a = %d, b = %d, f = %d\n ", a, b, top->f);
		assert(top->f == (a ^ b));
	}

    delete top;

    //delete contextp;
	
    return 0;
}
