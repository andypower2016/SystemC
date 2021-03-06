#ifndef NB_MASTER_H
#define NB_MASTER_H
#include "systemc.h"
#include "no_blocking_if.h"

SC_MODULE(nb_master)
{
	sc_in_clk clock;
	sc_port<no_blocking_if> bus_port;

	unsigned int m_start_addr;
	int m_timeout;
	SC_HAS_PROCESS(nb_master);

	nb_master(sc_module_name mn, unsigned int start_addr, int timeout);
	virtual ~nb_master() {}
	void run();
};
#endif