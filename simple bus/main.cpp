#include "top.h"

int sc_main(int argc, char ** argv)
{
  top top("top");
  sc_start(10, SC_NS);
  return 0;
}