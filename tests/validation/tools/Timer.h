#ifndef BENCHMARK_TIMER_H
#define BENCHMARK_TIMER_H

#include <iostream>
#include <sys/times.h>

class Timer {
public:
    /** Default constructor */
    Timer(const char* name):m_name(name) { reset(); }

    /** Start or restart the timer */
    void start() {
        times(&m_start);
    }

    /** Save end time and aggregate build-in clock */
    void stop() {
        times(&m_stop);
        m_stored.tms_utime += m_stop.tms_utime - m_start.tms_utime;
        m_stored.tms_stime += m_stop.tms_stime - m_start.tms_stime;
        m_start = m_stop;
    }

    /** Reset the clock */
    void reset() {
        m_stored.tms_utime = 0;
        m_stored.tms_stime = 0;
    }

    /** Print time elapsed */
    void print() {
        std::cout << m_name << ":" << std::endl;
        std::cout << "  user:   " << m_stored.tms_utime*10 << " ms" << std::endl;
        std::cout << "  system: " << m_stored.tms_stime*10 << " ms" << std::endl;
    }

private:
    const char *m_name;
    struct tms  m_start,m_stop,m_stored;
};

#endif
