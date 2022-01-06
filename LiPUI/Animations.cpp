#include "Animations.hpp"

LARGE_INTEGER Frequency;

void Animations::InitTimer()
{
	QueryPerformanceFrequency(&Frequency);
	_MESSAGE("Frequency %I64d", Frequency.QuadPart);
}

LARGE_INTEGER Animations::FromMilliseconds(UInt32 milliseconds)
{
	LARGE_INTEGER retVal;
	retVal.QuadPart = Frequency.QuadPart * milliseconds / 1000;
	return retVal;
}