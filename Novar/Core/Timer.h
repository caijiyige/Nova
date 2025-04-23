#pragma once
#include <iostream>
#include <chrono>

template <typename Fn>
class Timer
{
    
    public:


        Timer(const char* name,Fn&& func)
        :m_Name(name),m_func(func),m_Stopped(false)
        {
            m_start = std::chrono::high_resolution_clock::now();
        }
        void reset()
        {
            m_start = std::chrono::high_resolution_clock::now();
        }
        void Stop()
         {
            auto endPoint = std::chrono::high_resolution_clock::now();
            auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start).time_since_epoch().count();
            auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endPoint).time_since_epoch().count();
            float duration = (end - start) * 0.001f;

            m_Stopped = true;
            //std::cout << "Timer took: " <<  duration << " ms" << std::endl;
            m_func({m_Name,duration});
         }
        ~Timer()
        {
            if(!m_Stopped)
                Stop();
        }
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

        const char * m_Name;
        Fn m_func;
        bool m_Stopped;
        
};
