#include <unordered_map>
#include "ColorCoding.h"

#include "f4se/GameTypes.h"

std::unordered_map<UInt32, std::vector<Range>> cache;
BSReadWriteLock cacheLock;

// borders checks does not perform
void ColorCoding::SetRanges(UInt32 attributeId, std::vector<Range>&& ranges)
{
	BSWriteLocker lock(&cacheLock);
	cache[attributeId] = ranges;
}

UInt32 ColorCoding::GetColor(UInt32 attributeId, float value)
{
	BSReadLocker lock(&cacheLock);
	const auto ptr = cache.find(attributeId);
	if (ptr == cache.end())
	{
		//_MESSAGE("No scheme for %08X attribute", attributeId);
		return DefaultColor;
	}
	const std::vector<Range>& ranges = ptr->second;
	const size_t last = ranges.size() - 1;
	for (size_t i  = 0; i < ranges.size(); i++)
	{
		if (value < ranges[i].Border)
		{
			//_MESSAGE("Found color on index %d", i);
			return ranges[i].Color;
		}
		if (i == last)
		{
			//_MESSAGE("Returning color on last index");
			return ranges[last].Color;
		}
	}
	_MESSAGE("WTF?");
	throw;
}