#include "timer.h"
#include "timer_queue.h"
#include <assert.h>

using namespace green_turtle::collections;
using namespace green_turtle::network;

TimerQueue::TimerQueue(size_t slot_size,size_t interval):
    last_update_time_(0)
    ,interval_(interval)
    ,current_slot_(0)
    ,interval_exponent_(0)
{
  assert( ~(slot_size & (slot_size - 1)) && "Slot Size must be 2^n" );
  assert( ~(interval_ & (interval_ - 1)) && "Interval must be 2^n" );
  queues_.resize(slot_size);
  for(size_t idx = 0; idx < slot_size; ++idx)
    queues_[idx].set_deleted((Timer*)(void*)(-1l));
  for(uint64_t i = interval_; i != 0; i /= 2)
  {
    ++interval_exponent_;
  }
}
void TimerQueue::CancelTimer(Timer *timer_ptr)
{
  if(!timer_ptr || !timer_ptr->queue_)
    return;
  queue_type& queue = timer_ptr->queue_->queues_;
  size_t      slot = timer_ptr->iter_slot_;
  size_t      pos = timer_ptr->iter_pos_;

  if(slot < queue.size())
  {
    list_type& list_ = queue[slot];
    list_.erase(pos);
    timer_ptr->queue_ = NULL;
  }
}
void TimerQueue::ScheduleTimer(Timer *timer_ptr,uint64_t timer_interval,uint64_t time_delay)
{
  if(timer_ptr->IsInQueue())
    CancelTimer(timer_ptr);

  uint64_t  next_time = time_delay + timer_interval + last_update_time_;
  size_t    to_slot = current_slot_ + ((next_time + interval_ - 1) >> interval_exponent_);
  to_slot   = to_slot & (queues_.size() - 1);

  list_type& list_            = queues_[to_slot];
  timer_ptr->iter_pos_        = list_.insert(timer_ptr);
  timer_ptr->iter_slot_       = to_slot;
  timer_ptr->queue_           = this;
  timer_ptr->timer_interval_  = timer_interval;
  timer_ptr->next_handle_time_= next_time;
}
//TODO:egmkang
//change it with lambda expression when clang 3.1 release
struct ForEachInList{
  ForEachInList(TimerQueue& queue_,unordered_list<Timer*>& list_,uint64_t current_time)
      :queue_(queue_),list_(list_),current_time(current_time)
  {}
  bool operator() (Timer* timer,size_t iter)
  {
    if(timer)
    {
      while(timer && timer->GetNextHandleTime() <= current_time)
      {
        timer->HandleTimeOut();
        timer = list_.get(iter);
      }
      if(timer)
        queue_.ScheduleTimer(timer,timer->GetInterval(),0);
    }
    else
    {
      list_.erase(iter);
    }
    return true;
  }
  TimerQueue&             queue_;
  unordered_list<Timer*>& list_;
  uint64_t                current_time;
};
void TimerQueue::Update(uint64_t current_time)
{
  if(!last_update_time_) last_update_time_ = current_time;
  uint64_t delta = this->interval_ / 2;
  uint64_t delta_time = current_time + delta;
  while(last_update_time_ < delta_time)
  {
    list_type& list_ = queues_[current_slot_];
    ForEachInList for_each(*this,list_,delta_time);
    list_.for_each(for_each);
    
    current_slot_ = (current_slot_ + 1) & (queues_.size() - 1);
    last_update_time_ += interval_;   
  }
}