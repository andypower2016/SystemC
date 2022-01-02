#include "simple_bus.h"

simple_bus::simple_bus(sc_module_name mn)
	: sc_module(mn)
{
	SC_THREAD(handle_request);
	sensitive << clock.neg();
}

void simple_bus::read(int id, int *data, int addr)
{	
	dbg_print("read data %d from addr 0x%04x", *data, addr);
	//  TODO ... adds the master's request to request map
	


}

void simple_bus::write(int id, int *data, int addr)
{
	dbg_print("write data %d to addr 0x%04x", *data, addr);
}

bus_status simple_bus::get_bus_status()
{
	bus_status tmp;
	return tmp;
}

void simple_bus::handle_request()
{
	while(true)
	{
		wait();
		// TODO ... handles the master's request, interfacing with the slave
	}
}