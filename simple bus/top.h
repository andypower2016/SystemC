#ifndef TOP_H
#define TOP_H

#include "systemc.h"
#include "nb_master.h"
#include "simple_bus.h"
#include "slave.h"

SC_MODULE(top)
{
    // channels
    sc_clock clk;
    sc_clock slow_clk;

    // module instances
    nb_master *master;
    simple_bus *bus;
    slave *slv;

    // constructor
    SC_CTOR(top) :
      clk("ClkFast", 1, SC_NS),
      slow_clk("ClkSlow", 6, SC_NS, 0.5, 5*2, SC_NS, true)
    {
        // Create instances
      	master = new nb_master("nb master", 0x00, 500); // start from address 0x04, timeout=500ns
        bus = new simple_bus("bus");
        slv = new slave("slave", 0x00, 0xFF);

        // Clocks
        master->clock(clk);
        bus->clock(clk);
        slv->clock(slow_clk);

        // Bus interface with master
        master->bus_port(*bus);
        bus->slv_port(*slv);
    }

    // destructor
    ~top()
    {
      if (master) 
      {
        delete master; 
        master = 0;
      }
      if (bus) 
      {
        delete bus; 
        bus = 0;
      }
      if (slv) 
      {
        delete slv; 
        slv = 0;
      }
    }

}; // end class simple_bus_test


#endif