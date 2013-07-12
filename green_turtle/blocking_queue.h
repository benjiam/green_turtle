//Copyright 2012, egmkang wang.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of green_turtle nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// author: egmkang (egmkang@gmail.com)

#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__
#include <mutex>
#include <deque>
#include <noncopyable.h>

namespace green_turtle{

template<class T>
class BlockingQueue : NonCopyable
{
 public:
  typedef T value_type;
 public:
  bool Push(value_type&& v)
  {
    std::lock_guard<std::mutex> guard(mutex_);
    queue_write_.push_back(v);
    return true;
  }
  bool Push(const value_type& v)
  {
    std::lock_guard<std::mutex> guard(mutex_);
    queue_write_.push_back(v);
    return true;
  }

  bool Pop(value_type& v)
  {
    if(queue_read_.empty() && !queue_write_.empty())
    {
      std::lock_guard<std::mutex> guard(mutex_);
      std::swap(queue_read_, queue_write_);
    }
    if(!queue_read_.empty())
    {
      v = queue_read_.front();
      queue_read_.pop_front();
      return true;
    }
    return false;
  }
  uint64_t Size() const
  {
    //unsafe
    return queue_read_.size() + queue_write_.size();
  }
 private:
  std::deque<value_type>  queue_write_;
  std::deque<value_type>  queue_read_;
  std::mutex              mutex_;
};

}

#endif
