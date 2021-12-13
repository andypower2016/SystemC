#include "systemc.h"
#include "producer.h"
#include "consumer.h"
#include "fifo.h"

int sc_main(int argc, char* argv[])
{
  sc_clock ClkFast("ClkFast", 1, SC_NS);
  sc_clock ClkSlow("ClkSlow", 500, SC_NS, 0.5, 500*2, SC_NS, true);

  fifo fifo1;

  producer P1("P1");
  P1.out(fifo1);
  P1.Clock(ClkFast);

  consumer C1("C1");
  C1.in(fifo1);
  C1.Clock(ClkSlow);

  int redundant = 500;
  sc_start((500*18+1) + redundant, SC_NS);

  return 0;
}