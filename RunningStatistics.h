#ifndef RunningStatistics_h
#define RunningStatistics_h

#define RUNNINGStatistics_LIB_VERSION "0.0.01"

#include "Arduino.h"

class RunningStatistics
{
public:
    RunningStatistics(void);
    RunningStatistics(int);
    ~RunningStatistics();

    void clear();
    void addValue(unsigned long);
    void fillValue(unsigned long, int);

    float getAverage();
    float getStandardDeviation();

    unsigned long getElement(uint8_t idx);
    uint8_t getSize() { return _size; }
    uint8_t getCount() { return _cnt; }

protected:
    uint8_t _size;
    uint8_t _cnt;
    uint8_t _idx;
    unsigned long _sum;
    unsigned long * _ar;
};

#endif
