#pragma once
#include <unordered_map>
#include <vector>
#include "Helper3D.hpp"

struct WidgetState
{
	bool AutoPosition;
	std::string Name;
	bool Visible;
	std::vector<float> Values;
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

