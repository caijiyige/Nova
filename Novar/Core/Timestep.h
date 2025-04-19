#pragma once

namespace NV
{
    class Timestep
    {
        public: 
            Timestep(float time)
            :m_Time(time)
            {

            }

            float GetSeconds() const { return m_Time;}
            float GetMillseconds() const { return m_Time * 1000.0f ;}

        private:
            float m_Time;
    };
}