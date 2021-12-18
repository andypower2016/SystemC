#include "systemc.h"
#include <iomanip>

SC_MODULE(and2)
{
  SC_CTOR(and2)
  {
      SC_METHOD(DoAnd);
      sensitive << A << B;
  }

  void DoAnd()
  {
    F = A && B;
  }

  sc_in<bool> A, B;
  sc_out<bool> F;

};

SC_MODULE(not1)
{
  SC_CTOR(not1)
  {
      SC_METHOD(DoNot);
      sensitive << A;
  }

  void DoNot()
  {
    F = !A;
  }

  sc_in<bool> A;
  sc_out<bool> F;
};

SC_MODULE(decoder)
{
  SC_CTOR(decoder) : A3("a3"), A2("a2"),A1("a1"),A0("a0"),N1("n1"),N0("n0")
  {
    // binding i/o ports and wires
    N1.A(a1);
    N1.F(s1);

    N0.A(a0);
    N0.F(s0);
      
    A3.F(f3);
    A3.A(a1);
    A3.B(a0);

    A2.F(f2);
    A2.A(a1);
    A2.B(s0);

    A1.F(f1);
    A1.A(s1);
    A1.B(a0);

    A0.F(f0);
    A0.A(s1);
    A0.B(s0);
  }

  and2 A3, A2, A1, A0;
  not1 N1, N0;
  sc_signal<bool> s1, s0;
  sc_in<bool> a1, a0;
  sc_out<bool> f3, f2, f1, f0;
};

SC_MODULE(stim)
{

  SC_CTOR(stim)
  {
    SC_THREAD(stimGen);
    sensitive << Clk.pos();
    dont_initialize();
  }

  void stimGen()  // generates the input signal
  {
    a1.write(0);
    a0.write(0);
    wait();
    a1.write(0);
    a0.write(1);
    wait();
    a1.write(1);
    a0.write(0);
    wait();
    a1.write(1);
    a0.write(1);
    wait();

    sc_stop();
  } 
  sc_out<bool> a1, a0;
  sc_in<bool> Clk;
};

SC_MODULE(monitor)
{

  SC_CTOR(monitor)
  {
    SC_THREAD(print);
    sensitive << Clk.pos();
    dont_initialize();
  }

  void print()
  {
    std::cout << std::setw(10) << "Time";
    std::cout << std::setw(4) << "A1";
    std::cout << std::setw(4) << "A0";
    std::cout << std::setw(4) << "F3";
    std::cout << std::setw(4) << "F2";
    std::cout << std::setw(4) << "F1";
    std::cout << std::setw(4) << "F0" << std::endl;
    while(1)
    {
      wait();
      std::cout << std::setw(10) << sc_time_stamp();
      std::cout << std::setw(4) << a1.read();
      std::cout << std::setw(4) << a0.read();
      std::cout << std::setw(4) << f3.read();
      std::cout << std::setw(4) << f2.read();
      std::cout << std::setw(4) << f1.read();
      std::cout << std::setw(4) << f0.read() << std::endl;  
    }
  }

  sc_in<bool> a0, a1;
  sc_in<bool> f0, f1, f2, f3;
  sc_in<bool> Clk;
};

int sc_main(int argc, char* argv[])
{
  
  sc_signal<bool> a1, a0;
  sc_signal<bool> f0, f1, f2, f3;
  sc_clock Clk("clk", 1, SC_NS);

  stim s("stim");
  s.a1(a1);
  s.a0(a0);
  s.Clk(Clk);
  
  decoder d("decoder");
  d.a1(a1);
  d.a0(a0);
  d.f0(f0);
  d.f1(f1);
  d.f2(f2);
  d.f3(f3);

  monitor m("monitor");
  m.a1(a1);
  m.a0(a0);
  m.f0(f0);
  m.f1(f1);
  m.f2(f2);
  m.f3(f3);
  m.Clk(Clk);

  sc_start();
  return 0;
}