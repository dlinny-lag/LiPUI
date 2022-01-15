#include "LiPUIMenu.h"

#include <f4se/GameReferences.h>
#include <f4se/NiNodes.h>
#include "f4se/GameRTTI.h"
#include "f4se/GameCamera.h"
#include "f4se/GameSettings.h"

#include "Attributes.h"
#include "GFxHelper.hpp"
#include "History.hpp"
#include <cmath>

#include "BonesHelper.hpp"
#include "OverlapsResolver.hpp"
#include "Requests.hpp"
#include "RequestsContext.h"


RequestsContext rc;
std::vector<HighlightChanged> changesToAnimate;
UInt32 MaxDistance2 = 500*500;


struct LiPUIRequest : Request<RequestsContext>
{
};
RequestsProcessor<RequestsContext, LiPUIRequest> processor;

struct WidgetRequest : LiPUIRequest
{
	UInt32 ActorId;
	WidgetRequest(UInt32 actorId) : ActorId(actorId)
	{
		
	}
};

struct CreateWidgetRequest : WidgetRequest
{
	std::string Name;
	bool AutoPosition;

	CreateWidgetRequest(UInt32 actorId, const std::string& name, bool autoPos) : WidgetRequest(actorId), Name(name), AutoPosition(autoPos)
	{
	}
	bool Proceed(RequestsContext* context, GFxValue& stage, bool updateContextOnly) const override 
	{
		if (context->RegisteredWidgets.find(ActorId) != context->RegisteredWidgets.end())
		{
			_MESSAGE("Attempt to register widget twice for Actor %08X", ActorId);
			return false;
		}
		if (updateContextOnly)
		{ // just update state without UI updating
			const WidgetState state{AutoPosition, Name};
			context->RegisteredWidgets.emplace(std::make_pair(ActorId, state));
			return true;
		}
		GFxValue args[2];
		args[0].SetUInt(ActorId);
		args[1].SetString(Name.c_str());
		GFxValue ret;
		//_MESSAGE("Calling AddWidget for Actor %08X", w.ActorId);
		const bool result = stage.Invoke("AddWidget", &ret, args, 2);
		if (result)
		{
			const bool retVal = ret.GetBool();
			if (retVal)
			{
				const WidgetState state{AutoPosition, Name};
				context->RegisteredWidgets.emplace(std::make_pair(ActorId, state));
			}
			return retVal;
		}
		_MESSAGE("Failed to call AddWidget");
		return false;
	}

};
struct RemoveWidgetRequest : WidgetRequest
{
	RemoveWidgetRequest(UInt32 actorId) : WidgetRequest(actorId)
	{
		
	}
	bool Proceed(RequestsContext* context, GFxValue& stage, bool updateContextOnly) const override
	{
		if (context->RegisteredWidgets.find(ActorId) == context->RegisteredWidgets.end())
		{
			_MESSAGE("Attempt to remove unregistered widget for Actor %08X", ActorId);
			return false;
		}
		if (updateContextOnly)
		{ // just update state without UI updating
			context->RegisteredWidgets.erase(ActorId);
			return true;
		}
		GFxValue actorId;
		actorId.SetUInt(ActorId);
		GFxValue ret;
		//_MESSAGE("Calling RemoveWidget for Actor %08X", w.ActorId);
		const bool result = stage.Invoke("RemoveWidget", &ret, &actorId, 1);
		if (result)
		{
			const bool retVal = ret.GetBool();
			//_MESSAGE("RemoveWidget returned %s", retVal?"true":"false");
			if (retVal)
			{
				context->RegisteredWidgets.erase(ActorId);
			}
			return retVal;
		}
		_MESSAGE("Failed to call RemoveWidget");
		return false;
	}
};
struct SetAllValuesRequest : WidgetRequest
{
	SetAllValuesRequest(UInt32 actorId) : WidgetRequest(actorId)
	{
	}
	bool Proceed(RequestsContext* context, GFxValue& stage, bool updateContextOnly) const override
	{
		const auto ptr = context->RegisteredWidgets.find(ActorId);
		if (ptr == context->RegisteredWidgets.end())
		{
			_MESSAGE("Attempt to set values to unregistered widget for Actor %08X", ActorId);
			return false;
		}
		if (updateContextOnly)
		{ // just update state without UI updating
			return true;
		}
		GFxValue args[4];
		args[0].SetUInt(ActorId);

		const auto values = Attributes::GetValues(DYNAMIC_CAST(LookupFormByID(ActorId), TESForm, Actor));

		for(UInt32 i = 0; i < values.size(); i++)
		{
			char value[Attributes::StringLength];
			UInt32 color;
			if (!Attributes::PrintIndex(i, values[i], value, color))
			{
				_MESSAGE("Failed to generate value string");
				continue;
			}
			args[1].SetUInt(i);
			args[2].SetString(value);
			args[3].SetUInt(color);
			GFxValue ret;
			const bool result = stage.Invoke("SetActorValue", &ret, args, 4);
			if (result)
			{
				const bool retVal = ret.GetBool();
				if (retVal)
				{
					ptr->second.Visible = true; // same as in AS3
				}
				else
					_MESSAGE("%08X[%d]. SetActorValue returned false", ActorId, i);
			}
			else
				_MESSAGE("%08X[%d]. Failed to call SetActorValue", ActorId, i);

		}
		return true;
	}
};
struct ChangeValueRequest : WidgetRequest
{
	UInt32 AttributeId;
	float Value;
	ChangeValueRequest(UInt32 actorId, UInt32 attrId, float val) : WidgetRequest(actorId), AttributeId(attrId), Value(val)
	{
		
	}

	static void CreateOrReplaceAnimation(UInt32 actorId, UInt32 attributeIndex, UInt32 frameDuration, UInt32 framesCount)
	{
		auto iter = changesToAnimate.begin();
		while(iter != changesToAnimate.end())
		{
			if (iter->ActorId == actorId && iter->AttributeIndex == attributeIndex)
			{
				//_MESSAGE("replacing %08X[%d]", actorId, attributeIndex);
				changesToAnimate.erase(iter);
				return;
			}
			++iter;
		}
		changesToAnimate.emplace_back(HighlightChanged(actorId, attributeIndex, frameDuration, framesCount));
		//_MESSAGE("added %08X[%d]", actorId, attributeIndex);
	}
	bool Proceed(RequestsContext* context, GFxValue& stage, bool updateContextOnly) const override
	{
		const auto ptr = context->RegisteredWidgets.find(ActorId);
		if (ptr == context->RegisteredWidgets.end())
		{
			_MESSAGE("Attempt to change attribute %08X on unregistered widget for Actor %08X", AttributeId, ActorId);
			return false;
		}
		const SInt32 index = Attributes::GetIndex(AttributeId);
		if (index < 0)
		{
			_MESSAGE("Failed to find index of attribute %X08", AttributeId);
			return false;
		}
		if (updateContextOnly)
		{ // just update state without UI updating
			return true;
		}

		GFxValue args[4];
		args[0].SetUInt(ActorId);
		args[1].SetUInt(index);
		char value[Attributes::StringLength];
		UInt32 color;
		if (!Attributes::Print(AttributeId, Value, value, color))
		{
			_MESSAGE("Failed to print attribute %X08", AttributeId);
			return false;
		}
		args[2].SetString(value);
		args[3].SetUInt(color);
		//_MESSAGE("Set 0x%08X color for attribute %08X when value is %f", color, c.AttributeId, c.Value);

		GFxValue ret;
		//_MESSAGE("Calling SetActorValue for Actor %08X", c.ActorId);
		const bool result = stage.Invoke("SetActorValue", &ret, args, 4);
		if (result)
		{
			const bool retVal = ret.GetBool();
			if (retVal)
			{
				ptr->second.Visible = true;  // same as in AS3
				CreateOrReplaceAnimation(ActorId, index, 100, 5);
			}
			return true;
		}
		_MESSAGE("Failed to call SetActorValue");
		return false;
	}
};

struct PositionWidgetRequest : WidgetRequest
{
	SInt32 X;
	SInt32 Y;
	PositionWidgetRequest(UInt32 actorId, SInt32 x, SInt32 y) : WidgetRequest(actorId), X(x), Y(y)
	{
	}

	static bool CallSetWidgetPosition(GFxValue& stage, UInt32 actorId, SInt32 x, SInt32 y)
	{
		GFxValue args[3];
		args[0].SetUInt(actorId);
		args[1].SetInt(x);
		args[2].SetInt(y);
		GFxValue ret;
		//_MESSAGE("Calling SetWidgetPosition for Actor %08X. X = %d, Y = %d", actorId, x, y);
		const bool result = stage.Invoke("SetWidgetPosition", &ret, args, 3);
		if (result)
		{
			const bool retVal = ret.GetBool();
			if (!retVal)
				_MESSAGE("SetWidgetPosition returned false");
			return retVal;
		}
		_MESSAGE("Failed to call SetWidgetPosition");
		return false;
	}
	bool Proceed(RequestsContext* context, GFxValue& stage, bool updateContextOnly) const override
	{
		const auto ptr = context->RegisteredWidgets.find(ActorId);
		if (ptr == context->RegisteredWidgets.end())
		{
			_MESSAGE("Attempt to position unregistered widget for Actor %08X", ActorId);
			return false;
		}
		if (updateContextOnly || CallSetWidgetPosition(stage, ActorId, X, Y))
		{
			ptr->second.Screen.X = X;
			ptr->second.Screen.Y = Y;
		}
		return true;
	}
};

struct FontSizeRequest : LiPUIRequest
{
	float Size;
	FontSizeRequest(float size) : Size(size)
	{
		
	}
	bool Proceed(RequestsContext* context, GFxValue& stage, bool updateContextOnly) const override
	{
		if (updateContextOnly)
		{
			context->CurrentFontSize = Size;
			return true;
		}
		GFxValue sizeArg;
		sizeArg.SetNumber(Size);
		const bool result = stage.Invoke("SetFontSize", nullptr, &sizeArg, 1);
		if (result)
		{
			context->CurrentFontSize = Size;
			_MESSAGE("SetFontSize %f", Size);
			return true;
		}
		_MESSAGE("Failed to call SetFontSize");
		return false;
	}
};

constexpr double swfWidth = 800;
constexpr double swfHeight = 600;
double ratio = -1;
void UpdateAspectRatio(GFxValue& stage)
{
	if (ratio > 0)
		return;
	Setting* widthSetting = GetINISetting("iSize W:Display");
	Setting* heightSetting = GetINISetting("iSize H:Display");
	UInt32 width, height;
	if (widthSetting)
		width = widthSetting->data.u32;
	else
		return;
	if (heightSetting)
		height = heightSetting->data.u32;
	else
		return;
	if (width > 0 && height > 0)
	{
		_MESSAGE("Screen resolution %dx%d", width, height);
		// for 1920x1080 good scale is {0.5, 1}
		constexpr double goodScreenAspectRatio = 1920.0/1080.0;

		const double screenWidth = width;
		const double screenHeight = height;
		const double screenAspectRatio = screenWidth / screenHeight;
		const double horizontalScale = 0.5 * goodScreenAspectRatio / screenAspectRatio;
		_MESSAGE("horizontalScale = %f", horizontalScale);
		GFxValue arg;
		arg.SetNumber(horizontalScale);
		if (stage.Invoke("SetScaleX",nullptr, &arg, 1))
			ratio = horizontalScale;
		else
			_MESSAGE("Failed to call SetScaleX");
	}
}

bool UpdateFromGround = true;
void LiPUIMenu::ClearState()
{
	_MESSAGE("clearing state");
	rc.RegisteredWidgets.clear();
	changesToAnimate.clear();
	UpdateFromGround = true;
}

LiPUIMenu::~LiPUIMenu()
{
	self = nullptr;
	_MESSAGE("Destructor");
	UpdateFromGround = true;
}

void LiPUIMenu::CreateWidget(UInt32 actorId, const std::string& name, bool autoPosition)
{
	processor.Add(new CreateWidgetRequest(actorId, name, autoPosition));
	processor.Add(new SetAllValuesRequest(actorId));
}

void LiPUIMenu::RemoveWidget(UInt32 actorId)
{
	processor.Add(new RemoveWidgetRequest(actorId));
}
void LiPUIMenu::SetFontSize(float size)
{
	processor.Add(new FontSizeRequest(size));
	_MESSAGE("called SetFontSize %f", size);
}
void LiPUIMenu::ProcessChangeNotification(UInt32 actorId, UInt32 attributeId, float prevVal, float newVal, float exceed)
{
	if (rc.RegisteredWidgets.find(actorId) == rc.RegisteredWidgets.end())
		return;
	const AttributeType type = Attributes::GetAttributeType(attributeId);
	switch(type)
	{
		case AttributeType::Wrong:
			return;
		case AttributeType::Float:
			processor.Add(new ChangeValueRequest(actorId, attributeId, newVal));
			return;
		case AttributeType::Integer:
			if ((SInt32)prevVal != (SInt32)newVal || exceed > 0)
				processor.Add(new ChangeValueRequest(actorId, attributeId, newVal));
	}
}
void LiPUIMenu::SetWidgetPosition(UInt32 actorId, SInt32 x, SInt32 y)
{
	processor.Add(new PositionWidgetRequest(actorId, x, y));
}
void LiPUIMenu::SetDistance(UInt32 distance)
{
	MaxDistance2 = distance * distance;
	_MESSAGE("max distance set to %d", distance);
}



void UpdateFontColor(GFxValue& stage)
{
	BSFixedString mName("HUDMenu");
	HUDMenu * menu = dynamic_cast<HUDMenu*>((*g_ui)->GetMenu(mName));
	const UInt32 color = GFxHelper::GetTextColor(menu);
	GFxValue c;
	c.SetUInt(color);
	_MESSAGE("Set color = 0x%08X", color);
	if (!stage.Invoke("SetFontColor", nullptr, &c, 1))
	{
		_MESSAGE("Failed to call SetFontColor");
	}
}

LiPUIMenu* LiPUIMenu::self = nullptr;
void LiPUIMenu::SetVisible(bool visible)
{
	GFxValue v(visible);
	const bool res = stage.SetMember("visible", &v);
	if (!res)
		_MESSAGE("Failed to set visibility");
	if (visible)
	{
		UpdateFontColor(stage);
	}
}


void UpdateFromState()
{
	_MESSAGE("Updating from state");
	LiPUIMenu::SetFontSize(rc.CurrentFontSize);
	for(const auto& pair : rc.RegisteredWidgets)
	{
		LiPUIMenu::CreateWidget(pair.first, pair.second.Name, pair.second.AutoPosition);
		LiPUIMenu::SetWidgetPosition(pair.first, pair.second.Screen.X, pair.second.Screen.Y);
	}
	_MESSAGE("Update of %d widgets is scheduled", rc.RegisteredWidgets.size());
	rc.RegisteredWidgets.clear();
}

void LiPUIMenu::OnFrame()
{
	if (!movie)
	{
		_MESSAGE("No movie?");
		return;
	}
	if (!Attributes::Ready())
	{
		_MESSAGE("Cache not ready");
		return;
	}
	GFxValue initialized;
	if (!stage.Invoke("IsReady", &initialized, nullptr, 0))
	{
		_MESSAGE("Failed to call IsReady");
		return;
	}
	if (!initialized.GetBool())
	{
		_MESSAGE("Widget not ready");
		return;
	}

	UpdateAspectRatio(stage);

	if (UpdateFromGround)
	{
		if (!stage.Invoke("ClearWidgets", nullptr, nullptr, 0))
		{
			_MESSAGE("Failed to call ClearWidgets");
		}
		UpdateFontColor(stage);
	}

	processor.ProcessPendingRequests(&rc, stage, UpdateFromGround);
	if (UpdateFromGround)
	{
		UpdateFromState();
		UpdateFromGround = false;
	}
	ProcessAnimations();
	AutoPositionWidgets();
}


void LiPUIMenu::AdvanceMovie(float unk0, void * unk1)
{
	OnFrame();
	GameMenuBase::AdvanceMovie(unk0, unk1);
}



bool CallSetWidgetVisibility(GFxValue& stage, UInt32 actorId, bool visibility)
{
	GFxValue args[2];
	args[0].SetUInt(actorId);
	args[1].SetBool(visibility);
	GFxValue ret;
	if (!stage.Invoke("SetVisibility", &ret, args, 2))
	{
		_MESSAGE("Failed to call SetVisibility");
		return false;
	}
	const bool retVal = ret.GetBool();
	if (!retVal)
		_MESSAGE("SetVisibility returned false for actor %08X", actorId);
	return retVal;
}

void FillSize(GFxValue& stage, UInt32 actorId, Overlaps::Rectangle& rect)
{
	GFxValue arg;
	arg.SetUInt(actorId);
	GFxValue ret;
	if (!stage.Invoke("GetWidgetWidth",&ret, &arg, 1))
	{
		_MESSAGE("Failed to call GetWidgetWidth");
		rect.Width = -1;
	}
	else
	{
		rect.Width = ret.GetInt();
	}
	if (!stage.Invoke("GetWidgetHeight",&ret, &arg, 1))
	{
		_MESSAGE("Failed to call GetWidgetHeight");
		rect.Height = -1;
	}
	else
	{
		rect.Height = ret.GetInt();
	}
	//_MESSAGE("[%08X] size = {%d, %d}", actorId, rect.Width, rect.Height);
}


Point2D Project(NiPoint3& world, float width, float height)
{
	NiPoint3 posOut;
	WorldToScreen_Internal(&world, &posOut);
	posOut.y = 1.0f - posOut.y;
	posOut.y = height * posOut.y; 
	posOut.x = width * posOut.x + 15;
	const Point2D retVal{static_cast<SInt32>(posOut.x), static_cast<SInt32>(posOut.y)};
	return retVal;
}


bool TryGetCameraDetails(PlayerCameraDetails& out)
{
	TESCameraState* state = (*g_playerCamera)->cameraState;
	if (!state)
	{
		return false;
	}
	NiPoint3 pos;
	NiQuaternion rot;
	out.Mode = state->stateID;
	state->GetPosition(&pos);
	state->GetRotation(&rot);
	const Point3D p(pos);
	const Quaternion q(rot); // I believe it is normalized already 
	out.Position = p;
	out.Rotation = q;
	return true;
}

PlayerCameraDetails PreviousCamera;
void LiPUIMenu::AutoPositionWidgets()
{
	PlayerCameraDetails current;
	bool cameraChanged;
	if (!TryGetCameraDetails(current))
		cameraChanged = true;
	else
		cameraChanged = PreviousCamera != current;

	std::vector<Overlaps::Rectangle> rects;
	std::unordered_map<UInt32, Point2D> calculated;

	for (auto& pair : rc.RegisteredWidgets)
	{
		WidgetState& ws = pair.second;
		if (!ws.AutoPosition)
			continue;

		Actor* a =  DYNAMIC_CAST(LookupFormByID(pair.first), TESForm, Actor);
		if (!a)
		{
			_MESSAGE("Could not find actor %08X", pair.first);
			// remove widget for missing actor
			RemoveWidget(pair.first);
			continue;
		}

		UInt8 unk1 = 0;
		bool actorVisible = HasDetectionLOS(*g_player, a, &unk1);
		if (actorVisible)
		{
			NiAVObject * target = a->GetObjectRootNode();

			Point3D camPos;
			if (current.Mode == 0)
			{
				// in 1st person mode camera position is relative to player's position
				camPos = ((*g_player)->pos);
				camPos.X += current.Position.X;
				camPos.Y += current.Position.Y;
				camPos.Z += current.Position.Z;
			}
			else
			{
				camPos = current.Position;
			}
			Point3D dir(target->m_worldTransform.pos.x - camPos.X, target->m_worldTransform.pos.y - camPos.Y, target->m_worldTransform.pos.z - camPos.Z);
			if (dir.Length2() > MaxDistance2)
				actorVisible = false;
			else
			{
				const Point3D camForward = current.Rotation.Forward();
				if (camForward.Dot(dir) < 0)
					actorVisible = false; // prevent widget displaying if camera looks at opposing direction

				//camForward.Normalize();
				//dir.Normalize();
				//_MESSAGE("cam={%f, %f, %f}, dir={%f, %f, %f}. Q={%f, %f, %f, %f} vis=%s", 
				//	camForward.X, camForward.Y, camForward.Z, 
				//	dir.X, dir.Y, dir.Z, 
				//	current.Rotation.W, current.Rotation.X, current.Rotation.Y, current.Rotation.Z, 
				//	actorVisible?"true":"false");
			}

		}
		if (actorVisible)
		{
			GFxValue stageWidth, stageHeight;
			stage.Invoke("GetStageWidth", &stageWidth, nullptr, 0);
			stage.Invoke("GetStageHeight", &stageHeight, nullptr, 0);
			const SInt32 width = stageWidth.GetInt();
			const SInt32 height = stageHeight.GetInt();

			NiPoint3 headPos, actorPos;
			FindHeadPosition(a, headPos, actorPos);
			ws.HeadPosition = headPos;

			const bool actorStaying = 
				std::fabsf(actorPos.x - ws.ActorPosition.X) < 0.0001f &&
				std::fabsf(actorPos.y - ws.ActorPosition.Y) < 0.0001f;

			const Point2D screen = Project(headPos, width, height);

			const bool largeMove = std::abs(ws.Screen.X-screen.X) > 10 || std::abs(ws.Screen.Y-screen.Y) > 10;
			if (!cameraChanged && actorStaying && !largeMove)
			{
				// keep Z
				const float z = ws.ActorPosition.Z;
				ws.ActorPosition = actorPos;
				ws.ActorPosition.Z = z;
			}
			else
			{
				ws.ActorPosition = actorPos;
			}
			Point2D widgetPos = ws.Screen; // previous position
			Overlaps::Rectangle rect{pair.first, widgetPos.X, widgetPos.Y};
			FillSize(stage, pair.first, rect);
			// don't move widget if change is insignificant
			if (cameraChanged || !actorStaying || largeMove)
			{ 
				rect.X = screen.X;
				rect.Y = screen.Y;
				widgetPos = screen;
			}
			calculated[pair.first] = widgetPos;

			rects.emplace_back(rect);
			//_MESSAGE("[%08X]: {%d, %d}", rect.Id, rect.X, rect.Y);

		}
		else
		{
			if (ws.Visible)
			{
				if (CallSetWidgetVisibility(stage, pair.first, false))
				{
					ws.Visible = false;
				}
			}
		}
	}

	Overlaps::Resolver::Resolve(rects);

	for(const auto& resolved : rects)
	{
		WidgetState& ws = rc.RegisteredWidgets[resolved.Id];
		Point2D beforeResolving = calculated[resolved.Id];
		if (PositionWidgetRequest::CallSetWidgetPosition(stage, resolved.Id, resolved.X, resolved.Y))
		{
			ws.Screen.X = beforeResolving.X;
			ws.Screen.Y = beforeResolving.Y;
			ws.Width = resolved.Width;
			ws.Height = resolved.Height;
		}
		if (!ws.Visible)
		{
			if (CallSetWidgetVisibility(stage, resolved.Id, true))
				ws.Visible = true;
		}
	}

	PreviousCamera = current;
}

inline void SetActorValueAnimationState(GFxValue& stage, UInt32 actorId, UInt32 index, bool bold, float alpha)
{
	GFxValue args[4];
	args[0].SetUInt(actorId);
	args[1].SetUInt(index);
	args[2].SetBool(bold);
	args[3].SetNumber(alpha);
	GFxValue ret;
	if (!stage.Invoke("SetActorValueAnimationState", &ret, args, 4))
	{
		_MESSAGE("Failed to call SetActorValueAnimationState");
	}
	else
	{
		if (!ret.GetBool())
			_MESSAGE("SetActorValueAnimationState returned false");
	}
}
void LiPUIMenu::ProcessAnimations()
{
	auto anim = changesToAnimate.begin();
	while (anim != changesToAnimate.end())
	{
		if (anim->Expired())
		{
			const bool bold = anim->Advance();
			SetActorValueAnimationState(stage, anim->ActorId, anim->AttributeIndex, bold, 1); // restore alpha
		}
		if (anim->Finished())
		{
			anim = changesToAnimate.erase(anim);
		}
		else
			++anim;
	}
}

