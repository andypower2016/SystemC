#ifndef SLAVE_H
#define SLAVE_H
#include "systemc.h"
#include "slave_if.h"
#include "simple_bus_types.h"

class slave : public slave_if, public sc_module
{
public:
	sc_in_clk clock;

	unsigned int m_start_addr;
	unsigned int m_end_addr;
	int *mem;
	int size;

	SC_HAS_PROCESS(slave);
	slave(sc_module_name mn, unsigned int start, unsigned int end);
	virtual ~slave();
	void display_mem();
	bus_status read(int *data, unsigned int addr);
 	bus_status write(int *data, unsigned int addr);
 	
};
#endif