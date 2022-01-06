#pragma once

#include "PluginAPIExport.hpp"
#include "f4se/ScaleformMovie.h"
#include "f4se/GameMenus.h"
#include "f4se/GameReferences.h"

#include "LiPUIMenu.h"
#include "Attributes.h"

constexpr UInt32 PlayerActorId = 0x14;

std::vector<float> ToArray(VMArray<float>& values)
{
	std::vector<float> retVal;
	if (values.Length() == 0)
		return retVal;
	const UInt32 len = values.Length();
	retVal.reserve(len);
	for (UInt32 i = 0; i < len; i++)
	{
		float val;
		values.Get(&val, i);
		retVal.emplace_back(val);
	}
	return retVal;
}

namespace LiPUI
{
	bool EnableWidget(StaticFunctionTag* _, Actor* actor, BSFixedString name)
	{
		if (!actor)
			return false;
		// player's widget does not follow 3D models position
		LiPUIMenu::CreateWidget(actor->formID,name.c_str(), actor->formID != PlayerActorId);
		return true;
	};

	bool SetFontSize(StaticFunctionTag* _, float size)
	{
		LiPUIMenu::SetFontSize(size);
		return true;
	}

	bool DisableWidget(StaticFunctionTag* _, Actor* actor)
	{
		if (!actor)
			return false;
		LiPUIMenu::RemoveWidget(actor->formID);
		return true;
	}
	bool SetPlayerWidgetPosition(StaticFunctionTag* _, SInt32 x, SInt32 y)
	{
		LiPUIMenu::SetWidgetPosition(PlayerActorId, x, y);
		return true;
	}
	bool SetValues(StaticFunctionTag* _, Actor* actor, VMArray<float> values)
	{
		if (!actor)
			return false;

		const UInt32 len = values.Length();
		const UInt32 expectedLength = Attributes::Length();
		if (len != expectedLength)
		{
			_MESSAGE("SetValues: wrong values array with %d elements for Actor %08X. Expected %d", len, actor->formID, expectedLength);
			return false;
		}

		LiPUIMenu::SetValues(actor->formID, ToArray(values));
		return true;
	}
	bool ChangeValue(StaticFunctionTag* _, Actor* actor, ActorValueInfo* attribute, float newValue)
	{
		if (!actor)
			return false;
		if (!attribute)
			return false;
		LiPUIMenu::ChangeValue(actor->formID, attribute->formID, newValue);
		return false;
	}

	void SetMaxDistance(StaticFunctionTag* _, UInt32 distance)
	{
		LiPUIMenu::SetDistance(distance);
	}

	bool __RegisterAttributes(StaticFunctionTag* _, VMArray<ActorValueInfo*> attributes, VMArray<bool> flags)
	{
		return Attributes::BuildCache(attributes, flags);
	}

	void Register(VirtualMachine* vm)
	{
		vm->RegisterFunction(new NativeFunction2("__RegisterAttributes", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, __RegisterAttributes, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "__RegisterAttributes", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2("EnableWidget", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, EnableWidget, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "EnableWidget", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1("DisableWidget", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, DisableWidget, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "DisableWidget", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1("SetFontSize", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, SetFontSize, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "SetFontSize", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2("SetPlayerWidgetPosition", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, SetPlayerWidgetPosition, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "SetPlayerWidgetPosition", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction1("SetMaxDistance", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, SetMaxDistance, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "SetMaxDistance", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction2("SetValues", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, SetValues, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "SetValues", IFunction::kFunctionFlag_NoWait);

		vm->RegisterFunction(new NativeFunction3("ChangeValue", PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, ChangeValue, vm));
		vm->SetFunctionFlags(PluginAPIExport::EXPORT_PAPYRUS_SCRIPT, "ChangeValue", IFunction::kFunctionFlag_NoWait);

	}
}
