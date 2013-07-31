#include <logger.h>
#include <atomic>
#include <thread>
#include <stdlib.h>
#include <system.h>
#include <iostream>

const int kThreadCount = 1;
const long kMaxLogCount = 1000000;

int main()
{
    green_turtle::Logger logger("test.txt.1", "test.txt");
    std::atomic_long  count(0);
    std::thread *threads[kThreadCount];

    size_t begin = green_turtle::System::GetMilliSeconds();

    for(int i = 0; i < kThreadCount; ++i)
    {
        threads[i] = new std::thread([&](){
                while(true)
                {
                    logger.Debug("thread %zu write log, random_num %d", std::hash<std::thread::id>()(std::this_thread::get_id()), rand());
                    if(++count > kMaxLogCount)
                    {
                        break;
                    }
                }
                });
    }
    for(int i = 0; i< kThreadCount; ++i)
    {
        threads[i]->join();
        delete threads[i];
    }

    time_t end = green_turtle::System::GetMilliSeconds();
    std::cout << double(kMaxLogCount)/(end - begin) << std::endl;

    return 0;
}
