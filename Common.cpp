
#include "Common.hpp"

#include <glm/glm.hpp>
using namespace glm;


struct timespec GetTime()
{
    //Get the time using CLOCK MONOTONIC (time since a date in the past, unaffected by local time changes)
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return time;
}

void PrintDuration(struct timespec t1, struct timespec t2)
{
    //Calculate the total elapsed microseconds
    u32 microseconds = (t2.tv_sec*1000000u+t2.tv_nsec/1000u) - (t1.tv_sec*1000000u+t1.tv_nsec/1000u);
    //Convert to seconds and microseconds
    u32 seconds = microseconds/1000000u;
    microseconds = microseconds%1000000u;

    //Print the time
    printf("%u.%06u seconds", seconds, microseconds);

    //Calculate milliseconds
    u32 milliseconds = microseconds/1000u;
    microseconds = microseconds%1000u;
    //Print broken time
    printf(" (%us %um %uu)", seconds, milliseconds, microseconds);
}

int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
        {
            result *= base;
        }
        exp >>= 1;
        if (!exp)
        {
            break;
        }
        base *= base;
    }

    return result;
}
