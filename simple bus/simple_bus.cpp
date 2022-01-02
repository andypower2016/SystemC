#include "simple_bus.h"

simple_bus::simple_bus(sc_module_name mn)
	: sc_module(mn)
{
	SC_THREAD(handle_request);
	sensitive << clock.neg();
}

void simple_bus::read(int *data, int addr)
{	
	dbg_print("read data %d from addr %04x", *data, addr);
}
void simple_bus::write(int *data, int addr)
{
	dbg_print("write data %d to addr %04x", *data, addr);
}

bus_status simple_bus::get_bus_status()
{
	return m_status;
}

void simple_bus::handle_request()
{
	while(true)
	{
		wait();
	}
}