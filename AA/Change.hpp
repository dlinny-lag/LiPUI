#pragma once
#include "f4se/GameForms.h"
#include "f4se/PluginAPI.h"
#include <vector>

namespace Notifications
{
	constexpr UInt32 AttributeIncrementMessageType = 'AAI1';
	constexpr UInt32 AttributeDecrementMessageType = 'AAD1';

	struct Change
	{
		std::vector<TESForm*> senders; // NOTE: might contain nullptrs
		Actor* Actor;
		ActorValueInfo* Attribute;
		float PreviousValue;
		float NewValue;
		float Exceed;
	};

	/// <summary>
	/// Call this method to receive changes notifications. Method must be called after AA plugin loading. See 'Messaging API docs' in f4se/PluginAPI.h
	/// </summary>
	/// <param name="messaging">F4SEMessagingInterface for your plugin. Use QueryInterface in F4SEPlugin_Query to obtain pointer</param>
	/// <param name="plugin">Your plugin handle. use GetPluginHandle in F4SEPlugin_Query to obtain it</param>
	/// <param name="handler">Changes notifications handler</param>
	bool inline SubscribeForChanges(const F4SEMessagingInterface* messaging, PluginHandle plugin, F4SEMessagingInterface::EventCallback handler)
	{
		return messaging->RegisterListener(plugin, "AA", handler);
	}
}

/*
void ExampleMessageCallback(F4SEMessagingInterface::Message* msg)
{
	if (msg->type == Notifications::AttributeIncrementMessageType)
	{
		// some value increased
		ASSERT(sizeof(Notifications::Change) == msg->dataLen);
		Notifications::Change* change = static_cast<Notifications::Change*>(msg->data);
		// change pointer will be deleted almost immediately after this function return
		// so you have to get a copy if you want to proceed it later
	}
	throw "Don't use example in production";
}
*/