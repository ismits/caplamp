#include "RunningStatistics.h"
// #include <stdlib.h>

RunningStatistics::RunningStatistics(int n)
{
    _size = n;
    _ar = (unsigned long*) malloc(_size * sizeof(unsigned long));
    if (_ar == NULL) _size = 0;
    clear();
}

RunningStatistics::~RunningStatistics()
{
    if (_ar != NULL) free(_ar);
}

// resets all counters
void RunningStatistics::clear()
{
    _cnt = 0;
    _idx = 0;
    _sum = 0;
    for (int i = 0; i < _size; i++) _ar[i] = 0;
}

// adds a new value to the data-set
void RunningStatistics::addValue(unsigned long f)
{
    if (_ar == NULL) return;
    _sum -= _ar[_idx];
    _ar[_idx] = f;
    _sum += _ar[_idx];
    _idx++;
    if (_idx == _size) _idx = 0;  // faster than %
    if (_cnt < _size) _cnt++;
}

// returns the average of the data-set added sofar
float RunningStatistics::getAverage()
{
    if (_cnt == 0) return NAN;
    return _sum / _cnt;
}

float RunningStatistics::getStandardDeviation()
{
    float avg = getAverage();
    unsigned long var = 0;
    for (int i = 0; i < _cnt; i++) var += pow(_ar[i] - avg, 2);
    return sqrt(var / (_cnt - 1));
}


// returns the value of an element if exist, 0 otherwise
unsigned long RunningStatistics::getElement(uint8_t idx)
{
    if (idx >=_cnt ) return NAN;
    return _ar[idx];
}

// fill the average with a value
// the param number determines how often value is added (weight)
// number should preferably be between 1 and size
void RunningStatistics::fillValue(unsigned long value, int number)
{
    clear();
    for (int i = 0; i < number; i++)
    {
        addValue(value);
    }
}
