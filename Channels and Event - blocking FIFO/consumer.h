#ifndef CONS_H
#define CONS_H
#include "systemc.h"
#include "fifo_if.h"

class consumer : public sc_module
{
public:

  sc_port<fifo_in_if> in;
  sc_in<bool> Clock;

  void do_reads()
  {
    char c;
    while (true)
    {
      wait();             // for clock edge   
      if(in->read(c) > 0)
      {
        cout << "R " << c << " at "
             << sc_time_stamp() << endl;
      }
    }
  }

  SC_CTOR(consumer)
  {
    SC_THREAD(do_reads);
    sensitive << Clock.pos();
  }
};
#endif