#pragma once
#include <vector>
#include <profileapi.h>

class Animations
{
public:
	static void InitTimer();
	static LARGE_INTEGER FromMilliseconds(UInt32 milliseconds);
};

struct HighlightChanged
{
	UInt32 ActorId;
	UInt32 AttributeIndex;
	LARGE_INTEGER FrameDuration;
	LARGE_INTEGER LastUpdateTime;
	std::vector<bool> BoldState;
	HighlightChanged(UInt32 actorId, UInt32 attributeIndex, UInt32 milliseconds, size_t count):
		ActorId(actorId), AttributeIndex(attributeIndex)
	{
		ASSERT(count % 2);
		FrameDuration = Animations::FromMilliseconds(milliseconds);
		//_MESSAGE("Frame duration %I64d", FrameDuration.QuadPart);
		BoldState.reserve(count);
		for (size_t i = 0; i < count; i++)
		{
			BoldState.emplace_back(i % 2);
		}
		QueryPerformanceCounter(&LastUpdateTime);
	}
	bool Finished() const
	{
		return BoldState.empty();
	}
	bool Expired() const
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);
		const LONGLONG elapsed = current.QuadPart - LastUpdateTime.QuadPart;
		//_MESSAGE("%08X[%d]: %I64d elapsed ", ActorId, AttributeIndex, elapsed);
		return  elapsed > FrameDuration.QuadPart;
	}
	bool Finish()
	{
		return false;
	}
	bool Advance()
	{
		if (Finished())
			return false; // not bold
		const bool retVal = BoldState[0];
		BoldState.erase(BoldState.begin());
		QueryPerformanceCounter(&LastUpdateTime);
		//_MESSAGE("%08X[%d]: advanced to %s", ActorId, AttributeIndex, retVal?"true":"false");
		return retVal;
	}
};
