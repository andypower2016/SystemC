#ifndef SLAVE_IF_H
#define SLAVE_IF_H
#include "systemc.h"
#include "simple_bus_types.h"

class slave_if : public virtual sc_interface
{
public:
	virtual bus_status read(int *data, unsigned int addr)=0;
 	virtual bus_status write(int *data, unsigned int addr)=0;
};

#endif