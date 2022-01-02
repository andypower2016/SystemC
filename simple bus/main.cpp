##include "systemc.h"
#include "top.h"

int sc_main(int, char **)
{
  top top("top");
  sc_start(10000, SC_NS);
  return 0;
}