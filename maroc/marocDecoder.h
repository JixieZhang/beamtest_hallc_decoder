#pragma once

//
// A simple decoder to process the maroc data
// Reference: https://www.jlab.org/Hall-B/ftof/manuals/FADC250UsersManual.pdf
//
// Author: Jingyi Zhou, Chao Peng
// Date: 2020/09/01
//

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>


namespace maroc
{

class marocEvent
{
public:
    int Nedges, tTrigNum;
    double tTrigTime;
    std::vector<int> channel, edge, time, fiber, slot;

    marocEvent(size_t buf_size = 10000)
    : tTrigNum(0), tTrigTime(0), Nedges(0), channel(buf_size), edge(buf_size), time(buf_size), fiber(buf_size), slot(buf_size)
    {
        // place holder
    }

    void Clear() { Nedges = 0; }
};

// data type
enum marocDataType {
    BlockHeader = 0,
    BlockTrailer = 1,
    EventHeader = 2,
    TriggerTime = 3,
    // 4 - 6 reserved
    DeviceID = 7,
    TDCWord = 8,
    ADCWord = 9,
    // 10 - 13 reserved
    InvalidData = 14,
    FillerWord = 15,
};

class marocDecoder
{
public:
    // for an event data
    void DecodeEvent(marocEvent &event, const uint32_t *buf, size_t len) const;
};

}; // namespace maroc

