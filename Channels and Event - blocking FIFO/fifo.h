#ifndef FIFO_H
#define FIFO_H

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

public:
  // constructor
  explicit fifo(int size_ = 16)
  : sc_prim_channel(sc_gen_unique_name("myfifo"))
  {
    size = size_;
    buf = new char[size];
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
    if (num_free() == 0)
      wait(data_read_event);
    num_written++;
    buf[wi] = c;
    wi = (wi + 1) % size;
    free--;
    request_update();
  }

  void read(char& c)        // blocking read
  {
    if (num_available() == 0)
    {
      cout << __FUNCTION__ << " waiting for data at " << sc_time_stamp() << endl;
      wait(data_written_event);
    }
    num_read++;
    c = buf[ri];
    ri = (ri + 1) % size;
    free++;
    request_update();
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
      data_read_event.notify(SC_ZERO_TIME);
    if (num_written > 0)
      data_written_event.notify(SC_ZERO_TIME);
    num_readable = size - free;
    num_read = 0;
    num_written = 0;
  }

};
#endif