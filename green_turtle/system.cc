#include <sys/time.h>
#include <unistd.h>
#include "system.h"

using namespace green_turtle;

time_t System::GetSeconds()
{
  timeval time_val;
  gettimeofday(&time_val,NULL);

  return time_val.tv_sec;
}

uint64_t System::GetMilliSeconds()
{
  uint64_t time_millisecond;
  timeval time_val;
  gettimeofday(&time_val,NULL);
  time_millisecond = time_val.tv_sec * 1000 + time_val.tv_usec / 1000;
  return time_millisecond;
}

tm System::GetTime()
{
  tm time_tm;
  time_t time_sec = GetSeconds();
  localtime_r(&time_sec, &time_tm);
  return time_tm;
}

int System::GetSecondsDiffDays(time_t s1, time_t s2)
{
  tm tm_1;
  tm tm_2;
  localtime_r(&s1,&tm_1);
  localtime_r(&s2,&tm_2);
  tm_1.tm_sec = tm_1.tm_min = tm_1.tm_hour = 0;
  tm_2.tm_sec = tm_2.tm_min = tm_2.tm_hour = 0;
  s1 = mktime(&tm_1);
  s2 = mktime(&tm_2);
  return (s2 - s1) / (3600 * 24);
}

void System::Yield(uint64_t milliSeconds)
{
  ::usleep(milliSeconds*1000);
}

