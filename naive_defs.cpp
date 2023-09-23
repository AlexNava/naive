#include "naive_defs.hpp"


bool functions::isPowOf2(uint64_t number)
{
    return (getPowOf2(number) >= 0);
}

int8_t functions::getPowOf2(uint64_t number)
{
    if (number == 0)
        return -1;
    if ((number & (number - 1)) != 0)
        return -1;

    int8_t power = 0;
    while (number > 1)
    {
        number >>= 1;
        ++power;
    }
    return power;
}
