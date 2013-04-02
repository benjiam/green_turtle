#include <sys/time.h>
#include <unistd.h>
#include "system.h"

using namespace green_turtle;

time_t System::GetSeconds()
{
  timeval  time_val_;
  gettimeofday(&time_val_,NULL);
  return time_val_.tv_sec;
}

uint64_t System::GetMilliSeconds()
{
  timeval  time_val_;
  gettimeofday(&time_val_,NULL);
  uint64_t time_millisecond_ = time_val_.tv_sec * 1000 + time_val_.tv_usec / 1000;
  return time_millisecond_;
}

tm System::GetTime()
{
  time_t time_now = GetSeconds();
  tm time_tm;
  localtime_r(&time_now, &time_tm);
  return time_tm;
}

int System::GetSecondsDiffDays(time_t s1, time_t s2)
{
  tm tm_1_;
  tm tm_2_;
  localtime_r(&s1,&tm_1_);
  localtime_r(&s2,&tm_2_);
  tm_1_.tm_sec = tm_1_.tm_min = tm_1_.tm_hour = 0;
  tm_2_.tm_sec = tm_2_.tm_min = tm_2_.tm_hour = 0;
  s1 = mktime(&tm_1_);
  s2 = mktime(&tm_2_);
  return (s2 - s1) / (3600 * 24);
}

void System::Yield(uint64_t milliSeconds)
{
  ::usleep(milliSeconds*1000);
}

