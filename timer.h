#include "systemc.h"

SC_MODULE(timer)
{
	sc_inout<bool> start_timer;
	sc_out<bool> timeout;
	sc_in<bool> clock;

	int count;
	void runtimer();

	SC_CTOR(timer)
	{
		SC_THREAD(runtimer);
		sensitive << clock.pos();
		sensitive << start_timer;
		dont_initialize();
		count = 5;
	}

};

void timer::runtimer()
{
	while(1)
	{
		wait();
		if(start_timer)
		{
			cout << sc_time_stamp() << " timer start" << endl;
			count = 5;
			timeout = false;
			start_timer = false;
		}
		else
		{
			if(count > 0)
			{
				--count;
				timeout = false;
			}
			else
			{
				cout << sc_time_stamp() << " timer timeout" << endl;
				timeout = true;
			}
		}
		
	}
}