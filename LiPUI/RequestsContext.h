#pragma once
#include <unordered_map>
#include "Helper3D.hpp"

struct WidgetState
{
	bool AutoPosition;
	std::string Name;
	bool Visible;
	Point2D Screen;
	Point3D ActorPosition;
	Point3D HeadPosition;
	SInt32 Width;
	SInt32 Height;
};

struct RequestsContext
{
	float CurrentFontSize = 10;
	std::unordered_map<UInt32, WidgetState> RegisteredWidgets;
};

