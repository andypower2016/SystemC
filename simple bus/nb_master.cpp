#include "nb_master.h"

nb_master::nb_master(sc_module_name mn, unsigned int start_addr, int timeout)
    	: sc_module(mn), m_start_addr(start_addr), m_timeout(timeout)
{
	SC_THREAD(run);
	sensitive << clock.pos();
}


void nb_master::run()
{
	wait();
	
	int data = 1;
	int addr = m_start_addr;
	int idx_range = 5;	// indexs of int data
	int addr_size = (m_start_addr + 4 * idx_range);
	int id = 1;

	while(true)
	{

		bus_port->write(id, &data, addr);
		//wait for the bus to complete request
		while(bus_port->get_bus_status(id) != BUS_OK)
		{
			dbg_print("waiting for bus to complete");
			wait();
		}

		++data;
		addr = (addr + 4) % addr_size;
		if(addr == 0)
		{
			addr = m_start_addr;
		}
		// next clock pos edge
		wait();

		// breaks when writing all index once
		if(data > idx_range)
			break;
	}
}