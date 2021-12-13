#ifndef STACK_IF_H
#define STACK_IF_H
#include "systemc.h"

class stack_read_if : public sc_interface
{
public:
	virtual bool nb_read(char &c)=0;
};

class stack_write_if : public sc_interface
{
public:
	virtual bool nb_write(char c)=0;
	virtual void reset()=0;
};
#endif