#pragma once
#include "ColorCoding.h"
#include "f4se/ScaleformValue.h"

namespace GFxHelper
{
	// delimiter is '.' (dot)
	bool GetNestedMember(GFxValue* root, const std::string& path, GFxValue& out)
	{
		if (!root)
			return false;
		size_t start = 0;
		GFxValue* current = root;
		GFxValue m;
		do
		{
			const size_t pos = path.find('.', start);
			//_MESSAGE("pos = %d when start = %d", pos, start);
			if (pos == std::string::npos)
			{
				if (!current)
					return false;
				const std::string member = path.substr(start);
				//_MESSAGE("get last member %s", member.c_str());
				return current->GetMember(member.c_str(), &out);
			}
			const std::string member = path.substr(start, pos-start);
			//_MESSAGE("get value of member %s", member.c_str());
			const bool res = current->GetMember(member.c_str(), &m);
			if (!res)
			{
				_MESSAGE("Failed");
				return false;
			}
			current = &m;
			start = pos + 1;
		}
		while(true);
	}

	UInt32 GetTextColor(HUDMenu * menu)
	{
		UInt32 retVal = ColorCoding::DefaultColor;
		if (!menu)
		{
			_MESSAGE("Not a HUDMenu class ");
			return retVal;
		}
		GFxValue transform;
		if (!GetNestedMember(&menu->stage, "HUDNotificationsGroup_mc.Messages_mc.transform.colorTransform", transform))
		{
			_MESSAGE("Failed to get colorTransform");
			return retVal;
		}
		GFxValue red;
		if (!transform.GetMember("redMultiplier", &red))
		{
			_MESSAGE("Failed to get redMultiplier");
			return retVal;
		}
		GFxValue green;
		if (!transform.GetMember("greenMultiplier", &green))
		{
			_MESSAGE("Failed to get greenMultiplier");
			return retVal;
		}
		GFxValue blue;
		if (!transform.GetMember("blueMultiplier", &blue))
		{
			_MESSAGE("Failed to get blueMultiplier");
			return retVal;
		}
		retVal = static_cast<UInt32>(red.GetNumber()*255) << 16 |
				 static_cast<UInt32>(green.GetNumber()*255) << 8 |
				 static_cast<UInt32>(blue.GetNumber()*255);
		return retVal;
	}
	
}
