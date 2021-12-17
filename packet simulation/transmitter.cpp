#include "systemc.h"

SC_MODULE(timer)
{
	sc_inout<bool> start;
	sc_out<bool> timeout;	// outputs time out signal to transmitter
	sc_in<bool> clock;

	int count;
	void runtimer()
	{
		while(1)
		{
			if(start)
			{
				count = 0;
				timeout = false;
				start = false;
				std::cout << sc_time_stamp() << " timer start, timeout = " << timeout << std::endl;
			}
			else
			{
				if(count < 5)
				{
					++count;
					timeout = false;
				}
				else
				{
					timeout = true;
				}
			}
			wait();
		}
	}

	SC_CTOR(timer)
	{
		SC_THREAD(runtimer);
		sensitive << clock.pos();
		sensitive << start; 
		dont_initialize();
		count = 0;
	}

};

SC_MODULE(transmitter)
{
	sc_inout<bool> start;
	sc_in<bool> timeout;
	sc_in<bool> clock;

	void sendData()
	{
		if(timeout)
		{
			// restart timer
			cout << sc_time_stamp() << " timer timeout, restart timer, timeout = " << timeout << std::endl;
			start = true;
		}
		else
		{
			cout << sc_time_stamp() << " sending data, timeout =  " << timeout << endl;
			start = false;
		}
		
	}

	SC_CTOR(transmitter)
	{
		SC_METHOD(sendData);
		sensitive << clock.pos();
		sensitive << timeout;
		dont_initialize();
	}

};


SC_MODULE(Top)
{
	SC_CTOR(Top) : t1("t1"), trans1("trans1")
	{
		t1.timeout(timeout);
		t1.start(start);
		t1.clock(Clk);

		trans1.timeout(timeout);
		trans1.start(start);
		trans1.clock(Clk);

		t1.start = true;
		t1.timeout = false;
	}

	timer t1;
	transmitter trans1;

	sc_signal<bool> timeout;
	sc_signal<bool,SC_MANY_WRITERS> start;
	sc_in<bool> Clk;
};

int sc_main(int argc, char** argv)
{
	sc_clock clock("clock", 1, SC_NS, 0.5, 0, SC_NS, true);
	Top top("top1");
	top.Clk(clock);
	sc_start(20, SC_NS);
	sc_stop();
	return 0;
}