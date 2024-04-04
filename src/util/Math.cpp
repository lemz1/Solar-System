#include "Math.h"

float Math::Sign(float value)
{
	return (value > 0) ? +1
		:  (value < 0) ? -1
		:				  0;
}

float Math::Lerp(
	float a, 
	float b, 
	float t
)
{
    return a + t * (b - a);
}

float Math::Clamp(
	float value, 
	float min, 
	float max
)
{
    return (value < min) ? min
		:  (value > max) ? max
		:				 value;
}
