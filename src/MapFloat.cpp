#include "MapFloat.h"

float mapFloat(long x, long inMin, long inMax, float outMin, float outMax)
{
    return (x - inMin) * (outMax - outMin) /
           (float)(inMax - inMin) + outMin;
}