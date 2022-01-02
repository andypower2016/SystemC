#include "nb_master.h"

nb_master::nb_master(sc_module_name mn, unsigned int start_addr, timeout)
    	: sc_module(mn), m_start_addr(start_addr), m_timeout(timeout)
{
	SC_THREAD(run);
	sensitive << clock.pos();

	m_addr_range = 2;	// 32 indexs of int data
}

void nb_master::run()
{
	wait();
	int data;
	int addr = m_start_addr;
	int id = 1;
	while(true)
	{
		bus_port->write(id, &data, addr);
		addr = (addr + 4) % (m_start_addr + 4 * m_addr_range);
		if(addr == 0)
		{
			addr = m_start_addr;
		}
		wait();
	}
}