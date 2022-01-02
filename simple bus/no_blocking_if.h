#ifndef NB_IF_H
#define NB_IF_H
#include "systemc.h"
#include "simple_bus_types.h"

class no_blocking_if : public virtual sc_interface
{
public:
	virtual void read(int id, int *data, int addr)=0;
 	virtual void write(int id, int *data, int addr)=0;
 	virtual bus_status get_bus_status()=0;
}

#endif