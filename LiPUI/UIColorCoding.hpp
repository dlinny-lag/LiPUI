#pragma once
#include "f4se/PapyrusStruct.h"
#include "f4se/PapyrusNativeFunctions.h"
#include "ColorCoding.h"
#include <limits>
namespace UIColorCoding
{
	DECLARE_STRUCT(Interval, LIP_UI_PAPYRUS_SCRIPT);

	bool SetColorScheme(StaticFunctionTag* _, ActorValueInfo* attribute, VMArray<Interval> intervals)
	{
		if (!attribute)
		{
			_MESSAGE("None attribute");
			return false;
		}

		const UInt32 len = intervals.Length();
		if (len == 0)
			return false;

		std::vector<Range> ranges;
		ranges.reserve(len);
		float last = std::numeric_limits<float>::lowest();
		for (UInt32 i = 0; i < len; i++)
		{
			Interval interval;
			intervals.Get(&interval, i);
			if (interval.IsNone())
			{
				_MESSAGE("Interval %d in None", i);
				return false;
			}
			Range range;
			interval.Get("Border", &range.Border);
			if (len-1 != i && range.Border <= last)
			{
				_MESSAGE("Border[%d]= %f is less than previous border %f", i, range.Border, last);
				return false; // borders must have ascending order
			}
			interval.Get("Color", &range.Color);
			ranges.emplace_back(range);

			last = range.Border;
		}

		ColorCoding::SetRanges(attribute->formID, std::move(ranges));
		return true;
	}
	void Register(VirtualMachine* vm)
	{
		vm->RegisterFunction(new NativeFunction2("SetColorScheme", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, SetColorScheme, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "SetColorScheme", IFunction::kFunctionFlag_NoWait);
	}

	
}