#include "slave.h"

slave::slave(sc_module_name mn, unsigned int start, unsigned int end) : sc_module(mn)
{
	SC_METHOD(display_mem)
	sensitive << clock.pos();

	m_start_addr = start;
	m_end_addr = end;

	sc_assert(m_start_addr <= m_end_addr);
	sc_assert((m_end_addr - m_start_addr) % 4 == 0);

	size = (m_end_addr - m_start_addr + 1) / 4;
	mem = new int[size]();
}

slave::~slave() 
{
	if(mem)
	{
		delete []mem;
		mem = 0;
	}
}

void slave::display_mem()
{
	std::cout << sc_time_stamp() << " mem info" << std::endl;
	for(int i = 0 ; i < size ; ++i)
	{
		printf("mem[%d]=%d\n", i, mem[i]);
	}
}

bus_status slave::read(int *data, unsigned int addr)
{
	if(addr <= m_end_addr && addr >= m_start_addr)
	{
		int idx = (m_end_addr-m_start_addr)/4;
		dbg_print("read from slave, addr=0x%04x, idx=%d", addr, idx);
		*data = mem[idx];

		return BUS_OK;
	}
	return BUS_ERROR;
}

bus_status slave::write(int *data, unsigned int addr)
{
	if(addr <= m_end_addr && addr >= m_start_addr)
	{
		int idx = (m_end_addr-m_start_addr)/4;
		dbg_print("writing to slave, addr=0x%04x, idx=%d", addr, idx);
		mem[idx] = *data;

		return BUS_OK;
	}
	return BUS_ERROR;
}