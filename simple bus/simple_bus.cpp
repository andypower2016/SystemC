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
		ret = new request;
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
	request* req = get_request(id);
	if(req)
	{
		req->do_write = false;
		req->data = data;
		req->addr = addr;
		req->status = BUS_BUSY;
	}
}

void simple_bus::write(int id, int *data, int addr)
{
	dbg_print("write data %d to addr 0x%04x", *data, addr);
	request* req = get_request(id);
	if(req)
	{
		req->do_write = true;
		req->data = data;
		req->addr = addr;
		req->status = BUS_BUSY;
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
		request* req = next_request();
		if(req)
		{	
			bus_status slave_status;
			if(req->do_write)
			{
				slave_status = slv_port->write(req->data, req->addr);
			}
			else
			{
				slave_status = slv_port->read(req->data, req->addr);
			}
			switch(slave_status)
			{
				case BUS_OK:
					req->status = BUS_OK;
					break;
				default:
					req->status = BUS_ERROR;
					break;
			}
		}
	}
}