#ifndef BUS_TYPES_H
#define BUS_TYPES_H
#include "systemc.h"

#define dbg_print(format, args) std::cout << sc_time_stamp(); \
	printf(" [%s:%d] format" "\n", __FUNCTION__, __LINE__, ##args)

enum bus_status {
	BUS_BUSY,
	BUS_OK,
	BUS_ERROR,
}

#endif