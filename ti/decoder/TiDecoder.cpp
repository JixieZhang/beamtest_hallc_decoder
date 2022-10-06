//
// A simple decoder to process the JLab Ti data
//

#include "TiDecoder.h"

using namespace tidec;

#define SET_BIT(n,i)  ( (n) |= (1ULL << i) )
#define TEST_BIT(n,i)  ( (bool)( n & (1ULL << i) ) )

inline void print_word(uint32_t word)
{
    std::cout << "0x" << std::hex << std::setw(8) << std::setfill('0') << word << std::dec << "\n";
}

TiDecoder::TiDecoder()
{
    // place holder
}

void TiDecoder::DecodeEvent(const uint32_t *buf, size_t len)
{
    for(size_t i=0; i<len; i++)
        print_word(buf[i]);
}
