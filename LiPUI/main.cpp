#include <shlobj.h>
#include "f4se/PluginAPI.h"
#include "f4se_common/f4se_version.h"
#include "f4se/PapyrusVM.h"

#include "PluginAPIExport.hpp"
#include "LiPUI.hpp"
#include "LiPUIMenu.h"
#include "MenuOpenCloseHandler.h"
#include "UIColorCoding.hpp"
#include "StringAPI.hpp"
#include "AA/Change.hpp"

#define REQUIRED_RUNTIME RUNTIME_VERSION_1_10_163


PluginHandle g_pluginHandle = kPluginHandle_Invalid;

F4SEPapyrusInterface* g_papyrus = nullptr;
F4SEScaleformInterface *g_scaleform = nullptr;
//F4SESerializationInterface* g_serialization = nullptr;
F4SEMessagingInterface* g_messaging = nullptr;


bool RegisterExportingFunctions(VirtualMachine* vm)
{
	_MESSAGE("RegisterFuncs");
	PluginAPIExport::Register(vm);
	UIColorCoding::Register(vm);
	LiPUI::Register(vm);
	StringAPI::Register(vm);
	return true;
}

void Serialization_Revert(const F4SESerializationInterface* intfc)
{

}
void Serialization_Save(const F4SESerializationInterface* intfc)
{
}
void Serialization_Load(const F4SESerializationInterface* intfc)
{
}


template <class T>
void DispatchChangeMessage(T message)
{
	if (!message->Attribute)
	{
		_MESSAGE("None attribute");
		return;
	}
	if (!message->Actor)
	{
		_MESSAGE("None actor");
		return;
	}
	LiPUIMenu::ProcessChangeNotification(message->Actor->formID, message->Attribute->formID, message->PreviousValue, message->NewValue, message->Exceed);
}

void OnAttributeChanged(F4SEMessagingInterface::Message* msg)
{
	if (msg->type == Notifications::AttributeIncrementMessageType1 || msg->type == Notifications::AttributeDecrementMessageType1)
	{
		if (const Notifications::Change1* change = Notifications::GetAsVersion1(msg))
			DispatchChangeMessage(change);
	}
	else if(msg->type == Notifications::AttributeIncrementMessageType2 || msg->type == Notifications::AttributeDecrementMessageType2)
	{
		if (const Notifications::Change2* change = Notifications::GetAsVersion2(msg))
			DispatchChangeMessage(change);
	}
	else
		_MESSAGE("Invalid message type %d", msg->type);
}

void MessageCallback(F4SEMessagingInterface::Message* msg)
{
	if (msg->type == F4SEMessagingInterface::kMessage_PreLoadGame)
	{
		LiPUIMenu::ClearState();
	}
	else if (msg->type == F4SEMessagingInterface::kMessage_GameDataReady && msg->data)
	{
		_MESSAGE("Registering LiPUI");
		(*g_ui)->Register("LiPUI", LiPUIMenu::CreateMenu);
		
		MenuOpenCloseHandler::RegisterHandler();
	}
	else if(msg->type == F4SEMessagingInterface::kMessage_PostLoad)
	{
		if (!Notifications::SubscribeForChanges(g_messaging, g_pluginHandle, OnAttributeChanged))
		{
			_MESSAGE("Failed to subscribe to AA notifications");
		}
		else
		{
			_MESSAGE("Successfully subscribed for AA notifications");
		}
	}
}


extern "C"
{
	bool F4SEPlugin_Query(const F4SEInterface * f4se, PluginInfo * info)
	{
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Fallout4\\F4SE\\LiPUI.log");
		// populate info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = PluginAPIExport::pluginName;
		info->version = PluginAPIExport::pluginVersionInt;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = f4se->GetPluginHandle();

		if(f4se->isEditor)
		{
			_ERROR("Editor is not supported");

			return false;
		}

		if(f4se->runtimeVersion < REQUIRED_RUNTIME)
		{
			_ERROR("Unsupported runtime version %08X (expected %08X or higher)", f4se->runtimeVersion, REQUIRED_RUNTIME);

			return false;
		}

		g_papyrus = (F4SEPapyrusInterface *)f4se->QueryInterface(kInterface_Papyrus);
		if(!g_papyrus)
		{
			_ERROR("Failed to get F4SEPapyrusInterface");
			return false;
		}
		g_scaleform = (F4SEScaleformInterface *)f4se->QueryInterface(kInterface_Scaleform);
		if (!g_scaleform)
		{
			_ERROR("Failed to get F4SEScaleformInterface");
			return false;
		} 
		g_messaging = (F4SEMessagingInterface *)f4se->QueryInterface(kInterface_Messaging);
		if(!g_messaging)
		{
			_ERROR("Failed to get F4SEMessagingInterface");
			return false;
		}
		//g_serialization = (F4SESerializationInterface*)f4se->QueryInterface(kInterface_Serialization);
		//if (!g_serialization)
		//{
		//	_ERROR("Failed to get F4SESerializationInterface");
		//	return false;
		//}
		
		_MESSAGE("F4SEPlugin_Query successful.");
		return true;
	}

	bool F4SEPlugin_Load(const F4SEInterface * f4se)
	{
		//g_serialization->SetUniqueID(g_pluginHandle, PluginAPIExport::pluginUID);
		//g_serialization->SetRevertCallback(g_pluginHandle, Serialization_Revert);
		//g_serialization->SetSaveCallback(g_pluginHandle, Serialization_Save);
		//g_serialization->SetLoadCallback(g_pluginHandle, Serialization_Load);
		//g_serialization->SetFormDeleteCallback(g_pluginHandle, nullptr);


		g_papyrus->Register(RegisterExportingFunctions);
		g_scaleform->Register(LiPUIMenu::MenuName, LiPUIMenu::RegisterScaleform);
		g_messaging->RegisterListener(g_pluginHandle, "F4SE", MessageCallback);

		_MESSAGE("F4SEPlugin_Load successful.");
		return true;
	}

};
