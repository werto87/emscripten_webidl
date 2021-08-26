#ifndef D36D6A4C_6616_4E85_9BB2_212824D9378A
#define D36D6A4C_6616_4E85_9BB2_212824D9378A
#define BOOST_ASIO_DISABLE_THREADS // todo find out how to run multithreaded
#include <algorithm>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <chrono>
#include <cstdint>
#include <execution>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <utility>
#include <vector>

class Bar
{
public:
  Bar (long _val) : val (_val) { std::cout << "Bar(long val)" << val << std::endl; }
  unsigned long
  doSomething ()
  {
    boost::asio::io_context myContext{};

    return 42;
  }
  long val{};
};

#endif /* D36D6A4C_6616_4E85_9BB2_212824D9378A */
