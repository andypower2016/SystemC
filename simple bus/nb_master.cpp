#include "nb_master.h"

nb_master::nb_master(sc_module_name mn, unsigned int start_addr, timeout)
    	: sc_module(mn), m_start_addr(start_addr), m_timeout(timeout)
{
	SC_THREAD(run);
	sensitive << clock.pos();
}

void nb_master::run()
{
	wait();
	int data;
	int addr = m_start_addr;
	while(true)
	{
		bus_port->write(&data, addr);
		addr += 4;
		wait();
	}
}