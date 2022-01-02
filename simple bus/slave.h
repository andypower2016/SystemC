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

	SC_HAS_PROCESS(slave);
	slave(sc_module_name mn, unsigned int start, unsigned int end) : sc_module(mn)
	{
		SC_METHOD(display_mem)
		sensitive << clock.pos();

		m_start_addr = start;
		m_end_addr = end;

		sc_assert(m_start_addr <= m_end_addr);
		sc_assert((m_end_addr - m_start_addr) % 4 == 0);

		int size = (m_end_addr - m_start_addr + 1) / 4;
		mem = new int[size]();
	}

	virtual ~slave() 
	{
		if(mem)
		{
			delete []mem;
			mem = 0;
		}
	}

	void display_mem()
	{
		std::cout << sc_time_stamp() << " mem info" << std::endl;
		for(int i = 0 ; i < size ; ++i)
		{
			printf("mem[%d]=%d\n", i, mem[i]);
		}
	}

	void read(int *data, unsigned int addr)
	{
		if(addr <= m_end_addr && addr >= m_start_addr)
		{
			int idx = (m_end_addr-m_start_addr)/4;
			dbg_print("read from slave, addr=0x%04x, idx=%d", addr, idx);
			*data = mem[idx];
		}
	}
 	void write(int *data, unsigned int addr)
 	{
 		if(addr <= m_end_addr && addr >= m_start_addr)
		{
			int idx = (m_end_addr-m_start_addr)/4;
			dbg_print("writing to slave, addr=0x%04x, idx=%d", addr, idx);
			mem[idx] = *data;
		}
 	}
};
#endif