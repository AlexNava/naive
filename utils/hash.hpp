#ifndef NAIVE_HASH
#define NAIVE_HASH

#include <cstdint>

class NaiveHash
{
public:
    NaiveHash();
    void reset();
    void accumulate(uint64_t data);
    uint64_t get();

private:
    const uint64_t magic = 0x9e3779b97f4a7c15;
    uint64_t m_seed;
};

#endif
