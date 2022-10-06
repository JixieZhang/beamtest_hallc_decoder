#pragma once

//
// A simple decoder to process the JLab Ti data
//

#include <iostream>
#include <iomanip>
#include <map>
#include <vector>


namespace tidec
{

class TiDecoder
{
public:
    TiDecoder();

    // for an event data
    void DecodeEvent(const uint32_t *buf, size_t len);

private:
};

}; // namespace tidec
