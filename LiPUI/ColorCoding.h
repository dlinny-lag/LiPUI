#pragma once

struct Range
{
	float Border;
	UInt32 Color;
};

class ColorCoding
{
public:
	constexpr static UInt32 DefaultColor = 0x00FFFFFF;

	static void SetRanges(UInt32 attributeId, std::vector<Range>&& ranges);
	static UInt32 GetColor(UInt32 attributeId, float value);
};
