#include "systemc.h"
#include "stim.h"
#include "exor2.h"
//#include "mon.h"

#include <iomanip>

SC_MODULE(mon)
{
  sc_in<bool> A, B;
  sc_in<bool> F;
  sc_in<bool> Clk;

  void print_output()
  {
    std::cout << std::setw(10) << "Time";
    std::cout << std::setw(2) << "A";
    std::cout << std::setw(2) << "B";
    std::cout << std::setw(2) << "F" << std::endl;
    while(1)
    {
      std::cout << std::setw(10) << sc_time_stamp();
      std::cout << std::setw(2) << A.read();
      std::cout << std::setw(2) << B.read();
      std::cout << std::setw(2) << F.read() << std::endl;
      wait();
    }
  }

  SC_CTOR(mon)
  {
    SC_THREAD(print_output);
    sensitive << Clk.pos();
    dont_initialize();
  }
};

int sc_main(int argc, char* argv[])
{
  sc_signal<bool> ASig, BSig, FSig;
  sc_clock TestClk("TestClock", 10, SC_NS, 0.5, 1, SC_NS);

  stim Stim1("Stimulus");
  Stim1.A(ASig);
  Stim1.B(BSig);
  Stim1.Clk(TestClk);

  exor2 DUT("exor2");
  DUT.A(ASig);
  DUT.B(BSig);
  DUT.F(FSig);

  mon Monitor1("Monitor");
  Monitor1.A(ASig);
  Monitor1.B(BSig);
  Monitor1.F(FSig);
  Monitor1.Clk(TestClk);

  sc_start();  // run forever

  return 0;

}