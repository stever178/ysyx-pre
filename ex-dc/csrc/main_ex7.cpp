#include <assert.h>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Vex7.h"
#include <nvboard.h>
#include <verilated.h>
#include <verilated_vcd_c.h>

void nvboard_bind_all_pins(TOP_NAME *top);

#define MAX_SIM_TIME 1e7

// IBM PC AT
std::map<unsigned char, unsigned char> scancode_to_ascii_map = {
    // 字母和空格
    {0x1C, 'a'}, // ASCII: 0x61
    {0x32, 'b'}, // ASCII: 0x62
    {0x21, 'c'}, // ASCII: 0x63
    {0x23, 'd'}, // ASCII: 0x64
    {0x24, 'e'}, // ASCII: 0x65
    {0x2B, 'f'}, // ASCII: 0x66
    {0x34, 'g'}, // ASCII: 0x67
    {0x33, 'h'}, // ASCII: 0x68
    {0x43, 'i'}, // ASCII: 0x69
    {0x3B, 'j'}, // ASCII: 0x6A
    {0x42, 'k'}, // ASCII: 0x6B
    {0x4B, 'l'}, // ASCII: 0x6C
    {0x3A, 'm'}, // ASCII: 0x6D
    {0x31, 'n'}, // ASCII: 0x6E
    {0x44, 'o'}, // ASCII: 0x6F
    {0x4D, 'p'}, // ASCII: 0x70
    {0x15, 'q'}, // ASCII: 0x71
    {0x2D, 'r'}, // ASCII: 0x72
    {0x1B, 's'}, // ASCII: 0x73
    {0x2C, 't'}, // ASCII: 0x74
    {0x3C, 'u'}, // ASCII: 0x75
    {0x2A, 'v'}, // ASCII: 0x76
    {0x1D, 'w'}, // ASCII: 0x77
    {0x22, 'x'}, // ASCII: 0x78
    {0x35, 'y'}, // ASCII: 0x79
    {0x1A, 'z'}, // ASCII: 0x7A
    {0x29, ' '}, // ASCII: 0x20

    // 第一排数字键盘
    {0x0E, '`'}, // ASCII: 0x60
    {0x16, '1'}, // ASCII: 0x31
    {0x1E, '2'}, // ASCII: 0x32
    {0x26, '3'}, // ASCII: 0x33
    {0x25, '4'}, // ASCII: 0x34
    {0x2E, '5'}, // ASCII: 0x35
    {0x36, '6'}, // ASCII: 0x36
    {0x3D, '7'}, // ASCII: 0x37
    {0x3E, '8'}, // ASCII: 0x38
    {0x46, '9'}, // ASCII: 0x39
    {0x45, '0'}, // ASCII: 0x30
    {0x4E, '-'}, // ASCII: 0x2D
    {0x55, '='}  // ASCII: 0x3D
};

static void single_cycle(TOP_NAME *dut) {
  dut->clk = 0;
  dut->eval();
  dut->clk = 1;
  dut->eval();
}

static void exec_once(TOP_NAME *dut) { single_cycle(dut); }

static void reset(TOP_NAME *dut, int n) {
  dut->clrn = 0;
  while (n-- > 0) {
    single_cycle(dut);
  }
  dut->clrn = 1;
}

/*
module ex7(
    input clk,
    input clrn,
    input kbd_clk,
    input kbd_data,
    output ready, overflow, sampling,
    output [7:0] ps2_out,
    output [7:0] ascii_out,
    output reg [6:0] seg_count1, seg_count0,
    output reg [6:0] seg_ascii1, seg_ascii0,
    output reg [6:0] seg_scan_code1, seg_scan_code0,
    output reg [6:0] seg_empty1, seg_empty0
);
*/
struct DUT_INPUT {
  bool clk, clrn;
  bool kbd_clk, kbd_data;
} data_arr[] = {{}};

struct DUT_OUTPUT {
  bool ready, overflow, sampling;
  unsigned char ps2_out;
  unsigned char ascii_out;
  unsigned char seg_count1, seg_count0;
  unsigned char seg_ascii1, seg_ascii0;
  unsigned char seg_scan_code1, seg_scan_code0;
} ref_model;

void assign_dut(TOP_NAME *dut, const DUT_INPUT *ref_in) {
  // pass
}

void eval_ref(const DUT_INPUT *ref_in, DUT_OUTPUT *ref_out) {
  // pass
}

void print_data(const TOP_NAME *dut, const DUT_OUTPUT *ref_out,
                vluint64_t sim_time) {
  printf("cycle[%lu]\n", sim_time);

  printf("\t[dut input]  clk=%d clrn=%d kbd_clk=%d kbd_data=%d\n", dut->clk,
         dut->clrn, dut->kbd_clk, dut->kbd_data);
  printf("\t[dut output] ps2_out=%02x ascii_out=%02x\n", dut->ps2_out,
         dut->ascii_out);
}

void assert_ref(const TOP_NAME *dut, const DUT_OUTPUT *ref_out) {
  // pass
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
  size_t data_size = std::size(data_arr);
  assert(data_size > 0);
  // assert(data_size > 1);

  bool use_board = argc > 1 && strcmp(argv[1], "--use_board") == 0;
  if (true) {
    /* board */
    nvboard_bind_all_pins(dut);
    nvboard_init();

    dut->clk = 0;
    nvboard_update();
    reset(dut, 1);

    // while (sim_time < MAX_SIM_TIME) {
    while (1) {
      // if (sim_time % 1000 == 0) continue;

      dut->clk = !dut->clk;
      nvboard_update();

      // const auto test_pair = data_arr[sim_time % data_size];
      // assign_dut(dut, &test_pair);

      // eval_ref(&test_pair, &ref_model);
      dut->eval();

      if (dut->ready) {
        print_data(dut, &ref_model, sim_time);
        if (dut->ps2_out != 0xF0) {
          // assert(dut->ascii_out == scancode_to_ascii_map[dut->ps2_out]);
        }
      }

      assert_ref(dut, &ref_model);

      sim_time++;
    }
    nvboard_quit();
    m_trace->close();
    delete dut;
    return 0;
  }

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
