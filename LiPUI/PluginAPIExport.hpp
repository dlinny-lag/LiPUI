#pragma once

#include "f4se/PapyrusVM.h"
#include "f4se/PapyrusNativeFunctions.h"

#define LIP_UI_PAPYRUS_SCRIPT "LiP:UI"

namespace PluginAPIExport
{
	constexpr char* EXPORT_PAPYRUS_SCRIPT = LIP_UI_PAPYRUS_SCRIPT;

	const char* pluginName = "LiPUI";
	const UInt32 pluginUID = 'LiPU';
	const UInt32 pluginVersionInt = 0x0062;
	const char* pluginVersionString = "0.6.2";

	BSFixedString GetVersionString(StaticFunctionTag* _)
	{
		return pluginVersionString;
	}

	UInt32 GetVersionInt(StaticFunctionTag* _)
	{
		return pluginVersionInt;
	}

	void Register(VirtualMachine* vm)
	{
		vm->RegisterFunction(new NativeFunction0("GetVersionString", EXPORT_PAPYRUS_SCRIPT, GetVersionString, vm));
		vm->SetFunctionFlags(EXPORT_PAPYRUS_SCRIPT, "GetVersionString", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction0("GetVersionInt", EXPORT_PAPYRUS_SCRIPT, GetVersionInt, vm));
		vm->SetFunctionFlags(EXPORT_PAPYRUS_SCRIPT, "GetVersionInt", IFunction::kFunctionFlag_NoWait);
	}
}