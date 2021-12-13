#ifndef FIFO_H
#define FIFO_H

#include <mutex>

#include "systemc.h"
#include "fifo_if.h"

class fifo
: public sc_prim_channel, public fifo_out_if,
  public fifo_in_if
{
protected:
  int size;                 // size
  char* buf;                // fifo buffer
  int free;                 // free space
  int ri;                   // read index
  int wi;                   // write index
  int num_readable;
  int num_read;
  int num_written;

  sc_event data_read_event;
  sc_event data_written_event;

  std::mutex m_mutex;

public:
  // constructor
  explicit fifo(int size_ = 16)
  : sc_prim_channel(sc_gen_unique_name("myfifo"))
  {
    size = size_;
    buf = new char[size];
    num_read = num_written = 0;
    reset();
  }

  ~fifo()                   //destructor
  {
    delete [] buf;
  }

  int num_available() const
  {
    return num_readable - num_read;
  }

  int num_free() const
  {
    return size - num_readable - num_written;
  }

  void reset()
  {
    free = size;
    ri = 0;
    wi = 0;
  }


  void write(char c)        // blocking write
  {
    m_mutex.lock();
    if (num_free() == 0)
    {
      m_mutex.unlock();
      cout << __FUNCTION__ << " queue is full at " << sc_time_stamp() << endl;  
      wait(data_read_event);
      m_mutex.lock();
    }
    num_written++;
    buf[wi] = c;
    wi = (wi + 1) % size;
    free--;
    request_update();
    m_mutex.unlock();
  }

  void read(char& c)        // blocking read
  {
    m_mutex.lock();
    if (num_available() == 0)
    {
      m_mutex.unlock();
      cout << __FUNCTION__ << " waiting for data at " << sc_time_stamp() << endl;  
      wait(data_written_event);
      m_mutex.lock();
    }
    num_read++;
    c = buf[ri];
    ri = (ri + 1) % size;
    free++;  
    request_update();
    m_mutex.unlock();
  }

  char read()
  {
    char c;
    read(c);
    return c;
  }

  void update()
  {
    if (num_read > 0)
    {
      cout << __FUNCTION__ << " data_read_event notify " << sc_time_stamp() << endl; 
      data_read_event.notify(SC_ZERO_TIME);
    }
    if (num_written > 0)
    {
      cout << __FUNCTION__ << " data_written_event notify " << sc_time_stamp() << endl; 
      data_written_event.notify(SC_ZERO_TIME);
    }
    num_readable = size - free;
    num_read = 0;
    num_written = 0;
  }

};
#endif