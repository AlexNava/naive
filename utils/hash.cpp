#include "hash.hpp"

NaiveHash::NaiveHash()
{
    reset();
}

void NaiveHash::reset()
{
    m_seed = 0;
}

void NaiveHash::accumulate(uint64_t data)
{
    m_seed ^= (data + magic + (m_seed << 12) + (m_seed >> 4));
}

uint64_t NaiveHash::get()
{
    return m_seed;
}
