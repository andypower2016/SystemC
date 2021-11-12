#include "systemc.h"

SC_MODULE(ATM)
{

  void Person()
  {
    while(true)
    {
      wait();
      withdraw = m_withdraw.read();
      e_withDraw.notify(SC_ZERO_TIME);

      cout <<"@" << sc_time_stamp() << " withdrawing money, withdraw ammount=" << withdraw << std::endl;
      cout <<"@" << sc_time_stamp() << " waiting for atm to complete ..." << std::endl;
      wait(e_withDrawComplete);
    }

  }
  
  void AtmHandler()
  { 
    while(true)
    {
      wait();
      if(total < withdraw)
      {
        total = withdraw * 2;
      }
      total -= withdraw;
      wait(5, SC_NS);
      cout <<"@" << sc_time_stamp() << " atm complete withdraw" << std::endl;
      m_total.write(total);
      e_withDrawComplete.notify(SC_ZERO_TIME);
    }
  }

  void DisplayTotal()
  {
     cout <<"@" << sc_time_stamp() <<
        " :: Total Value "<< m_total.read() << endl;
  }

  SC_CTOR(ATM) 
  {    
    SC_THREAD(AtmHandler);
    sensitive << e_withDraw;

    SC_THREAD(Person);
    sensitive << m_clock.pos();
    
    SC_METHOD(DisplayTotal);
    sensitive << m_total;
    dont_initialize();

    total = 31;
  }

  // local vars
  sc_uint<4> withdraw;
  sc_uint<5> total;

  // ATM 
  sc_event e_withDraw;
  sc_event e_withDrawComplete;
  sc_in<bool> m_clock;
  sc_in<sc_uint<4>> m_withdraw;
  sc_out<sc_uint<5>> m_total;
  
};


int sc_main(int argc, char* argv[])
{
  ATM atm("ATM");

  sc_clock clock("clock", 1, SC_NS, 0.5, 0, SC_NS, true);
  sc_signal<sc_uint<4>> withdraw;
  sc_signal<sc_uint<5>> total;

  atm.m_withdraw(withdraw);
  atm.m_clock(clock);
  atm.m_total(total);

  // trace file
  sc_trace_file *wf = sc_create_vcd_trace_file("./atm_trace");
  sc_trace(wf, clock, "clock");
  sc_trace(wf, withdraw, "withdraw");
  sc_trace(wf, total, "total");

  withdraw = 10;
  for(int i = 0 ; i < 10 ; ++i)
  {
    sc_start(1, SC_NS);
  }
  withdraw = 1;
  for(int i = 0 ; i < 10 ; ++i)
  {
    sc_start(1, SC_NS);
  }

  sc_close_vcd_trace_file(wf);
  sc_stop();

  return 0;

}