#ifndef PROD_H
#define PROD_H
#include "systemc.h"
#include "fifo_if.h"

class producer : public sc_module
{
public:

  sc_port<fifo_out_if> out;
  sc_in<bool> Clock;

  int count = 0;

  void do_writes()
  {
    int i = 0;
    char* TestString = "abcdefghijklmnop";
    while (true)
    {
      wait();             // for clock edge
      if(out->write(TestString[i]) > 0)
      {
        cout << "W " << TestString[i] << " at "
             << sc_time_stamp() << endl;

        i = (i+1) % 16;
      }
      if(count++==16) // breaks, producer totaly writes "abcdefghijklmnopa"
        break;
    }
  }

  SC_CTOR(producer)
  {
    SC_THREAD(do_writes);
      sensitive << Clock.pos();
  }
};
#endif