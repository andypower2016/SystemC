#ifndef PROD_H
#define PROD_H
#include "systemc.h"
#include "stack_if.h"

class producer : public sc_module
{
public:

  sc_port<stack_write_if> out;
  sc_in<bool> Clock;

  void do_writes()
  {
    int i = 0;
    char* TestString = "Hallo,     This Will Be Reversed";
    while (true)
    {
      wait();             // for clock edge
      if (out->nb_write(TestString[i]));
        /*cout << "W " << TestString[i] << " at "
             << sc_time_stamp() << endl;*/
      i = (i+1) % 32;
      if(i == 0) // write TestString once
      {
        out->SetDone(true);
        break;
      }
    }
  }

  SC_CTOR(producer)
  {
    SC_THREAD(do_writes);
      sensitive << Clock.pos();
  }
};
#endif