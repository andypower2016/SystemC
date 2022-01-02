#ifndef BUS_H
#define BUS_H
#include "systemc.h"
#include "no_blocking_if.h"

#include <map>

typedef struct request
{
	int id;
	bool do_write;
	int *data;
	int addr;
	bus_status status;
} request;

class simple_bus : public no_blocking_if, public sc_module
{
public:
	sc_in_clk clock;
	
	SC_HAS_PROCESS(simple_bus);

	simple_bus(sc_module_name mn);
  	virtual ~simple_bus();
  	void handle_request();
  	request* get_request(int id);
protected:
	virtual void read(int id, int *data, int addr);
 	virtual void write(int id, int *data, int addr);
 	virtual bus_status get_bus_status();

private:
	std::map<int, request*> m_bus_requests;
}
#endif