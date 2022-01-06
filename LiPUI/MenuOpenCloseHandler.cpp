#include "MenuOpenCloseHandler.h"
#include "LiPUIMenu.h"

const std::vector<std::string_view> requiresClosed =
{
	"PauseMenu",
	"MainMenu",
};
const std::vector<std::string_view> requiresHidden =
{
	"LoadingMenu",
	"FaderMenu",
	"MessageBoxMenu",
	"ScopeMenu",
	"PipboyMenu",
	"VignetteMenu",
	"WorkshopMenu",
	"ContainerMenu",
	"SleepWaitMenu",
	"CookingMenu",
	"RobotModMenu",
	"BarterMenu",
	"LockpickingMenu",
	"BookMenu",
	"SPECIALMenu",
	"FavoritesMenu",
	"VATSMenu",
	"PipboyHolotapeMenu",
};

struct LiPMenuState
{
	bool Open;
	bool Visible;
	static LiPMenuState CalculateState()
	{
		LiPMenuState retVal {true, true};
		for (const auto closed : requiresClosed)
		{
			BSFixedString mName(closed.data());
			if ((*g_ui)->IsMenuOpen(mName))
			{
				retVal.Open = false;
				return retVal;
			}
		}
		for(const auto hidden: requiresHidden)
		{
			BSFixedString mName(hidden.data());
			if ((*g_ui)->IsMenuOpen(mName))
			{
				retVal.Visible = false;
				return retVal;
			}
		}
		return retVal;
	}
};


MenuOpenCloseHandler MenuOpenCloseHandler::menuOpenCloseHandler;

EventResult	MenuOpenCloseHandler::ReceiveEvent(MenuOpenCloseEvent * evn, void * dispatcher)
{
	const char * name = evn->menuName.c_str();
	
	_MESSAGE("Menu %s is %s", name, evn->isOpen?"opening":"closing");

	if (strcmp(LiPUIMenu::MenuName, name) == 0)
		return kEvent_Continue;

	const LiPMenuState state = LiPMenuState::CalculateState();
	if (!state.Open)
	{
		LiPUIMenu::CloseMenu();
	}
	else if (!state.Visible)
	{
		LiPUIMenu::HideMenu();
	}
	else
		LiPUIMenu::OpenMenu();

	return kEvent_Continue;
}

void MenuOpenCloseHandler::RegisterHandler()
{
	(*g_ui)->menuOpenCloseEventSource.RemoveEventSink(&menuOpenCloseHandler);
	(*g_ui)->menuOpenCloseEventSource.AddEventSink(&menuOpenCloseHandler);
}