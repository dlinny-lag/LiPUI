#pragma once
#include "f4se/GameMenus.h"
#include "f4se/ScaleformLoader.h"

#include <vector>
#include <unordered_map>

#include "f4se/PapyrusArgs.h"
#include "Animations.hpp"

class LiPUIMenu final : public GameMenuBase
{
	static LiPUIMenu* self;
	LiPUIMenu()
	{
		depth = 0;
		flags = kFlag_AllowSaving;
		const bool result = CALL_MEMBER_FN((*g_scaleformManager), LoadMovie)(this, movie, MenuName, "root1", 2);
		_MESSAGE("LiPUIMenu loaded: %s", result?"true":"false");
		Animations::InitTimer();
	}
public:

	static inline const char* MenuName = "LiPUI";

	~LiPUIMenu() override;
	LiPUIMenu(const LiPUIMenu&) = delete;
	LiPUIMenu(const LiPUIMenu&&) = delete;
	LiPUIMenu& operator=(const LiPUIMenu&) = delete;
	LiPUIMenu& operator=(LiPUIMenu&&) = delete;

	static void CreateWidget(UInt32 actorId, const std::string& name, bool autoPosition);
	static void SetValues(UInt32 actorId, const std::vector<float>& values);
	static void RemoveWidget(UInt32 actorId);
	static void SetFontSize(float size);
	static void ChangeValue(UInt32 actorId, UInt32 attributeId, float value);
	static void SetWidgetPosition(UInt32 actorId, SInt32 x, SInt32 y);
	static void ClearState();
	static void SetDistance(UInt32 distance);
	static void OpenMenu()
	{
		BSFixedString mName(MenuName);
		if (!(*g_ui)->IsMenuOpen(mName))
		{
			_MESSAGE("Opening LiPUIMenu...");
			CALL_MEMBER_FN((*g_uiMessageManager), SendUIMessage)(mName, kMessage_Open);
		}
		else
		{
			if (self)
				self->SetVisible(true);
		}
	}

	static void HideMenu()
	{
		if (self)
			self->SetVisible(false);
	}

	static void CloseMenu()
	{
		BSFixedString mName(MenuName);
		CALL_MEMBER_FN((*g_uiMessageManager), SendUIMessage)(mName, kMessage_Close);
	}

	static IMenu* CreateMenu()
	{
		_MESSAGE("calling LiPUIMenu()");
		self = new LiPUIMenu();
		return self;
	}

	static bool RegisterScaleform(GFxMovieView * view, GFxValue * f4se_root)
	{
		GFxMovieRoot* movieRoot = view->movieRoot;
		if (!movieRoot)
			return false;

		GFxValue loadingSwfPath;
		if (!movieRoot->GetVariable(&loadingSwfPath, "root.loaderInfo.url"))
		{
			_ERROR("Failed to get root.loaderInfo.url");
			return false;
		}
		_MESSAGE("Loaded SWF - %s", loadingSwfPath.GetString());
		std::string_view swfName(loadingSwfPath.GetString());
		if (swfName.find(MenuName) == std::string::npos)
			return false;
		// handle our menu loading

		return true;
	}
	void AdvanceMovie(float unk0, void * unk1) override;

private:
	void SetVisible(bool);
	void ProcessAnimations();
	void AutoPositionWidgets();
	void OnFrame();
};

