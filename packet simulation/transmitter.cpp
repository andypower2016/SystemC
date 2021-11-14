#include "timer.h"

// fake trasmitter, simulating timeout
SC_MODULE(transmitter)
{
	sc_inout<bool> start_timer;
	sc_in<bool> timeout;
	sc_in<bool> clock;

	void sendData();

	SC_CTOR(transmitter)
	{
		SC_METHOD(sendData);
		sensitive << clock.pos();
		sensitive << timeout;
	}

};

void transmitter::sendData()
{
	if(timeout)
	{
		// restart timer
		cout << sc_time_stamp() << " restart timer" << endl;
		start_timer = true;
	}
	else
	{
		// do nothing
		cout << sc_time_stamp() << " not sending data" << endl;
	}
}


int sc_main(int argc, char** argv)
{
	sc_signal<bool> timeout;
	sc_signal<bool,SC_MANY_WRITERS> start;
	sc_clock clock("clock", 1, SC_NS, 0.5, 0, SC_NS, true);

	timer t("timer");
	t.start_timer(start);
	t.timeout(timeout);
	t.clock(clock);

	transmitter trans("transmitter");
	trans.start_timer(start);
	trans.timeout(timeout);
	trans.clock(clock);

	sc_start(20, SC_NS);
	sc_stop();
	return 0;
}
