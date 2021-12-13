#ifndef FIFO_IF_H
#define FIFO_IF_H
#include "systemc.h"

class fifo_out_if :  virtual public sc_interface
{
public:
  virtual void write(char) = 0;          // blocking write
  virtual int num_free() const = 0;      // free entries
protected:
  fifo_out_if()
  {
  };
private:
  fifo_out_if (const fifo_out_if&);      // disable copy
  fifo_out_if& operator= (const fifo_out_if&); // disable
};

class fifo_in_if :  virtual public sc_interface
{
public:
  virtual void read(char&) = 0;          // blocking read
  virtual char read() = 0;
  virtual int num_available() const = 0; // available
                                         // entries
protected:
  fifo_in_if()
  {
  };
private:
  fifo_in_if(const fifo_in_if&);            // disable copy
  fifo_in_if& operator= (const fifo_in_if&); // disable =
};

#endif