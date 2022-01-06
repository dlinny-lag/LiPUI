#pragma once
#include "f4se/GameMenus.h"

class MenuOpenCloseHandler : public BSTEventSink<MenuOpenCloseEvent>
{
public:
	EventResult	ReceiveEvent(MenuOpenCloseEvent * evn, void * dispatcher) override;
	static void RegisterHandler();
private:
	static MenuOpenCloseHandler menuOpenCloseHandler;
};
