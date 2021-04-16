
#include "../Common.hpp"


Time GetTime()
{
	//Get the time using CLOCK MONOTONIC (time since a date in the past, unaffected by local time changes)
	Time time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time;
}

std::string DurationString(Time t1, Time t2)
{
	//Calculate the total elapsed microseconds
	u32 microseconds = (t2.tv_sec*1000000u+t2.tv_nsec/1000u) - (t1.tv_sec*1000000u+t1.tv_nsec/1000u);
	//Convert to seconds and microseconds
	u32 seconds = microseconds/1000000u;
	microseconds = microseconds%1000000u;

	std::string outStr;

	char buffer[128];

	//Print the time
	sprintf(buffer, "%u.%06u seconds", seconds, microseconds);
	outStr += buffer;

	//Calculate milliseconds
	u32 milliseconds = microseconds/1000u;
	microseconds = microseconds%1000u;
	//Print broken time
	sprintf(buffer, " (%us %um %uu)", seconds, milliseconds, microseconds);
	outStr += buffer;

	return outStr;
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
