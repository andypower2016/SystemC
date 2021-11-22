#include "systemc.h"

const int size = 32;
const int atm_op_time = 5; // atm operation time (ns)

SC_MODULE(ATM)
{

  void DummyManWithDraw()
  {
    while(true)
    {
      wait();
      if(!bDeposit)
      {
        withdraw = rand() % 1000;
        m_Withdraw.write(withdraw);
        eWithDraw.notify(SC_ZERO_TIME);
        cout <<"@" << sc_time_stamp() << " DummyManWithDraw() withdrawing money, withdraw ammount=" << withdraw << std::endl;
        cout <<"@" << sc_time_stamp() << " DummyManWithDraw() waiting for atm to complete ..." << std::endl;
        wait(eWithDrawComplete);
      } 
    }
  }

  void DummyManDeposit()
  {
    while(true)
    {
      wait(eDeposit);
      int deposit = rand() % 2000;
      total += deposit;
      cout <<"@" << sc_time_stamp() << " DummyManDeposit() depositing money, deposit ammount=" << deposit << std::endl;
      wait(atm_op_time, SC_NS);
      eDepositComplete.notify(SC_ZERO_TIME);
    }
  }
  
  void AtmHandler()
  { 
    m_AccountTotal.write(total);
    while(true)
    {
      wait(eWithDraw);

      // deposit if total < withdraw
      if(total < withdraw)
      {
        bDeposit = true;
        eDeposit.notify(SC_ZERO_TIME);
        wait(eDepositComplete);
        bDeposit = false;
        
        char msg[100];
        sprintf(msg, "AtmHandler() Deposit Complete ! Account Total = %d", total);
        cout <<"@" << sc_time_stamp() << msg << std::endl;
      }
      
      // continue withdraw after deposit
      total -= withdraw;
      wait(atm_op_time, SC_NS);
      cout <<"@" << sc_time_stamp() << "AtmHandler() atm complete withdraw" << std::endl;
      m_AccountTotal.write(total);
      eWithDrawComplete.notify(SC_ZERO_TIME);
    }
  }

  void DisplayTotal()
  {
     cout <<"@" << sc_time_stamp() <<
        " DisplayTotal() Account total = "<< m_AccountTotal.read() << endl;
  }

  SC_CTOR(ATM) 
  {    
    SC_THREAD(AtmHandler);

    SC_THREAD(DummyManWithDraw);
    sensitive << m_clock.pos();

    SC_THREAD(DummyManDeposit);
    
    total = 1000;
    bDeposit = false;
    SC_METHOD(DisplayTotal);
    sensitive << m_AccountTotal;
  }

  // local vars
  int withdraw;
  int total;
  bool bDeposit;

  // ATM 
  sc_event eDeposit;
  sc_event eDepositComplete;

  sc_event eWithDraw;
  sc_event eWithDrawComplete;

  sc_in<bool> m_clock;
  sc_out<sc_uint<size>> m_Deposit;
  sc_out<sc_uint<size>>  m_Withdraw;
  sc_out<sc_uint<size>> m_AccountTotal;
  
};


int sc_main(int argc, char* argv[])
{
  ATM atm("ATM");

  sc_clock clock("clock", 1, SC_NS, 0.5, 0, SC_NS, true);
  sc_signal<sc_uint<size>> withdraw;
  sc_signal<sc_uint<size>> deposit;
  sc_signal<sc_uint<size>> account_total;

  atm.m_Withdraw(withdraw);
  atm.m_clock(clock);
  atm.m_AccountTotal(account_total);
  atm.m_Deposit(deposit);

  // trace file
  sc_trace_file *wf = sc_create_vcd_trace_file("./atm_trace");
  sc_trace(wf, clock, "clock");
  sc_trace(wf, withdraw, "withdraw");
  sc_trace(wf, account_total, "account_total");
  sc_trace(wf, deposit, "deposit");

  sc_start(100, SC_NS);
  sc_close_vcd_trace_file(wf);
  sc_stop();

  return 0;

}