#include "simple_bus.h"

simple_bus::simple_bus(sc_module_name mn)
	: sc_module(mn)
{
	SC_THREAD(handle_request);
	sensitive << clock.neg();
}

request* simple_bus::get_request(int id)
{
	request* ret = nullptr;
	auto iter = m_bus_requests.find(id);
	if(iter == m_bus_requests.end())
	{
		request* ret = new request;
		m_bus_requests[id] = ret;
	}
	else
	{
		ret = iter->second;
	}	
	return ret;
}	

request* simple_bus::next_request()
{
	for(auto& req : m_bus_requests)
	{	
		if(req.second->status == BUS_BUSY)
		{
			return req.second;
		}
	}
	return nullptr;
}

void simple_bus::read(int id, int *data, int addr)
{	
	dbg_print("read data %d from addr 0x%04x", *data, addr);
	auto request = get_request(id);
	if(request)
	{
		request->do_write = false;
		request->data = data;
		request->addr = addr;
		request->status = BUS_BUSY;
	}
}

void simple_bus::write(int id, int *data, int addr)
{
	dbg_print("write data %d to addr 0x%04x", *data, addr);
	auto request = get_request(id);
	if(request)
	{
		request->do_write = true;
		request->data = data;
		request->addr = addr;
	}
}

bus_status simple_bus::get_bus_status(int id)
{
	auto iter = m_bus_requests.find(id);
	if(iter == m_bus_requests.end())
	{
		return BUS_STATUS_NOT_FOUND;
	}	
	return iter->second->status;
}

void simple_bus::handle_request()
{
	while(true)
	{
		wait();
		auto request = next_request();
		if(request)
		{	
			bus_status slave_status;
			if(request->do_write)
			{
				slave_status = slv_port->write(request->data, request->addr);
			}
			else
			{
				slave_status = slv_port->read(request->data, request->addr);
			}
			switch(slave_status)
			{
				case BUS_OK:
					request->status = BUS_OK;
				default:
					request->status = BUS_ERROR;
					break;
			}
		}
	}
}