#pragma once
#include "PluginAPIExport.hpp"
#include "f4se/PapyrusNativeFunctions.h"

namespace StringAPI
{
	BSFixedString ToHexString(StaticFunctionTag* _, UInt32 number)
	{
		char buffer[20];
		snprintf(buffer, 20, "0x%08X", number);
		return BSFixedString(buffer);
	}

	UInt32 FromHexString(StaticFunctionTag* _, BSFixedString value)
	{
		const char* str = value.c_str();
		if (!str)
			return 0;
		UInt32 retVal;
		if (sscanf_s(str,"0x%08X", &retVal) != 1)
			return 0;
		return retVal;
	}

	void Register(VirtualMachine* vm)
	{
		vm->RegisterFunction(new NativeFunction1("ToHexString", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, ToHexString, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "ToHexString", IFunction::kFunctionFlag_NoWait);
		vm->RegisterFunction(new NativeFunction1("FromHexString", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, FromHexString, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "FromHexString", IFunction::kFunctionFlag_NoWait);
	}
}