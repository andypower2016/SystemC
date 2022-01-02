#ifndef TOP_H
#define TOP_H

#include "systemc.h"
#include "nbmaster.h"
#include "simple_bus.h"

SC_MODULE(simple_bus_test)
{
    // channels
    sc_clock clk;

    // module instances
    nb_master *nb_master;
    simple_bus *simple_bus;

    // constructor
    SC_CTOR(top)
      : clk("clk")
    {
        // Create instances
      	nb_master = new nb_master("nb master", 0x04, 500); // start from address 0x04, timeout=500ns
        simple_bus = new simple_bus("bus");

        // Clocks
        nb_master->clock(clk);
        simple_bus->clock(clk);

        // Bus interface with master
        nb_master->bus_port(*simple_bus);

    }

    // destructor
    ~top()
    {
      if (nb_master) 
      {
        delete nb_master; 
        nb_master = 0;
      }
      if (simple_bus) 
      {
        delete simple_bus; 
        simple_bus = 0;
      }
    }

}; // end class simple_bus_test


#endif