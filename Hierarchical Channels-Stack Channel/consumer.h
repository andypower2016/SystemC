#ifndef CONS_H
#define CONS_H
#include "systemc.h"
#include "stack_if.h"

class consumer : public sc_module
{
public:

  sc_port<stack_read_if> in;
  sc_in<bool> Clock;

  void do_reads()
  {
    char c;
    while (true)
    {
      wait();             // for clock edge

      if(in->GetDone() && in->IsEmpty())
      {
        cout << endl;
        break;
      }

      if (in->nb_read(c))
        //cout << "R " << c << " at "
        //     << sc_time_stamp() << endl;
        cout << c ;
    }
  }

  SC_CTOR(consumer)
  {
    SC_THREAD(do_reads);
    sensitive << Clock.pos();
  }
};
#endif