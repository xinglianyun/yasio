//////////////////////////////////////////////////////////////////////////////////////////
// A cross platform socket APIs, support ios & android & wp8 & window store universal app
//
//////////////////////////////////////////////////////////////////////////////////////////
/*
The MIT License (MIT)

Copyright (c) 2012-2019 halx99

Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
Copyright (c) 2008 Roelof Naude (roelof.naude at gmail dot com)

Distributed under the Boost Software License, Version 1.0. (See accompanying
file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#ifndef YASIO__SOCKET_SELECT_INTERRUPTER_HPP
#define YASIO__SOCKET_SELECT_INTERRUPTER_HPP
#include "yasio/xxsocket.hpp"
#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

namespace yasio
{
namespace inet
{
class socket_select_interrupter
{
public:
  // Constructor.
  inline socket_select_interrupter();

  // Destructor.
  inline ~socket_select_interrupter();

  // Recreate the interrupter's descriptors. Used after a fork.
  inline void recreate();

  // Interrupt the select call.
  inline void interrupt();

  // Reset the select interrupt. Returns true if the call was interrupted.
  inline bool reset();

  // Get the read descriptor to be passed to select.
  socket_native_type read_descriptor() const { return read_descriptor_; }

private:
  // Open the descriptors. Throws on error.
  inline void open_descriptors();

  // Close the descriptors.
  inline void close_descriptors();

  // The read end of a connection used to interrupt the select call. This file
  // descriptor is passed to select such that when it is time to stop, a single
  // byte will be written on the other end of the connection and this
  // descriptor will become readable.
  socket_native_type read_descriptor_;

  // The write end of a connection used to interrupt the select call. A single
  // byte may be written to this to wake up the select which is waiting for the
  // other end to become readable.
  socket_native_type write_descriptor_;
};

} // namespace inet
} // namespace yasio

#include "socket_select_interrupter.ipp"

#endif // YASIO__SOCKET_SELECT_INTERRUPTER_HPP
