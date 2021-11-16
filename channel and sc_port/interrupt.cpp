#include "systemc.h"
// 2021.11.14 Chuo-Yu Yang
// An example of using sc_ports to connect to channel.
// A channel implements the interface functions, such as Interrupt in this example.

struct Interrupt : public sc_interface
{
  void notify()
  {
	  m_interrupt.notify();
  }
  void notify(sc_time t) 
  {
      m_interrupt.notify(t);
  }
  const sc_event& default_event() const override 
  { 
      return m_interrupt; 
  }
  sc_event m_interrupt;
};

struct Source : public sc_module
{
  SC_HAS_PROCESS(Source);
  Source(const std::string& nm) : sc_module(nm)
  {
	  SC_THREAD(SendEvent);
  }
  void SendEvent()
  { 
	  while(1)
	  {
		  wait(2, SC_NS);	
		  irq_op->notify(SC_ZERO_TIME);
	  }
  }
  sc_port<Interrupt> irq_op;
};

struct Destination : public sc_module
{
  SC_HAS_PROCESS(Destination);
  Destination(const std::string& nm) : sc_module(nm)
  {
	   SC_THREAD(DetectEvent);	
  }
  void DetectEvent()
  {
	  while(1)
	  {
		 wait(irq_ip->default_event());	
		 std::cout << sc_time_stamp() << " Destination Detect event" << std::endl;
	  }
  }
  sc_port<Interrupt> irq_ip;
};

class RunSimulation
{
public:
	RunSimulation()
	{
		m_irq = new Interrupt();
		m_source = new Source("Source");
		m_dest = new Destination("Destination");
		m_source->irq_op(*m_irq);
		m_dest->irq_ip(*m_irq);
	}
	~RunSimulation()
	{
		Destruct();
		std::cout << "RunSimulation::Dtor" << std::endl;
	}
	void Destruct()
	{
		if(m_irq) 
			delete m_irq;
		if(m_source) 
			delete m_source;
		if(m_dest) 
			delete m_dest;
	}
private:
	Interrupt *m_irq;
	Source *m_source;
  Destination *m_dest;
};

int sc_main(int argc, char* argv[])
{
  RunSimulation run;
  sc_start(20, SC_NS);
  sc_stop();
  return 0;
}