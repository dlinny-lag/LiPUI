#include "Attributes.h"
#include "f4se/GameReferences.h"
#include "ColorCoding.h"
#include "f4se/GameRTTI.h"

class IntValue final : public PrintableValue
{
public:
	IntValue(UInt32 index) : PrintableValue(index)
	{
		
	}
	bool Print(float value, char* output) const override
	{
		const SInt32 val = static_cast<SInt32>(value);
		if (val < -99 || val > 999)
			return false;
		snprintf(output, 4, "%3ld", val);
		return true;
	}
	AttributeType GetType() const override
	{
		return AttributeType::Integer;
	}
};
class FloatValue final: public PrintableValue
{
public:
	FloatValue(UInt32 index) : PrintableValue(index)
	{
		
	}
	bool Print(float value, char* output) const override final
	{
		snprintf(output, Attributes::StringLength, "%.3f", value);
		return true;
	}
	AttributeType GetType() const override
	{
		return AttributeType::Float;
	}
};


BSReadWriteLock Attributes::lockObject;
std::unordered_map<UInt32, std::unique_ptr<PrintableValue>> Attributes::cache;
std::vector<UInt32> Attributes::allAttributes;

bool Attributes::Ready()
{
	BSWriteLocker lock(&lockObject);
	return !cache.empty(); 
}
UInt32 Attributes::Length()
{
	BSWriteLocker lock(&lockObject);
	return cache.size();
}

AttributeType Attributes::GetAttributeType(UInt32 attributeId)
{
	BSWriteLocker lock(&lockObject);
	const auto ptr = cache.find(attributeId);
	if (ptr == cache.end())
		return AttributeType::Wrong;
	return ptr->second->GetType();
}

bool Attributes::BuildCache(VMArray<ActorValueInfo*>& attributes, VMArray<bool>& flags)
{
	BSWriteLocker lock(&lockObject);
	cache.clear();
	allAttributes.clear();

	const UInt32 len = attributes.Length();
	if (len != flags.Length())
		return false;
	
	for (UInt32 i = 0; i < len; i++)
	{
		ActorValueInfo* attribute;
		attributes.Get(&attribute, i);
		if (!attribute)
		{
			_MESSAGE("None attribute on index %d", i);
			cache.clear();
			allAttributes.clear();
			return false;
		}
		bool flag;
		flags.Get(&flag, i);
		PrintableValue* value;
		if (flag)
			value = new FloatValue(i);
		else
			value = new IntValue(i);
		allAttributes.emplace_back(attribute->formID);
		cache.emplace(std::make_pair(attribute->formID, std::unique_ptr<PrintableValue>(value)));
	}
	_MESSAGE("Cache built");
	return true;
}

float GetValueCore(Actor* actor, const UInt32 attributeId)
{
	float retVal = 0;
	bool found = false;
	for (UInt32 i = 0; i < actor->actorValueData.count; i++)
	{
		if (attributeId == actor->actorValueData[i].avFormID)
		{
			retVal = actor->actorValueData[i].value;
			found = true;
			break;
		}
	}
	if (!found)
	{ // should almost never happens
		ActorValueInfo* avi = DYNAMIC_CAST(LookupFormByID(attributeId), TESForm, ActorValueInfo);
		return actor->actorValueOwner.GetValue(avi);
	}

	for (UInt64 i = 0; i < actor->unk350.count; i++)
	{
		if (attributeId == actor->unk350[i].avFormID)
		{
			retVal += actor->unk350[i].unk04 + actor->unk350[i].unk08 + actor->unk350[i].unk0C;
			break;
		}
	}

	return retVal;
}

std::vector<float> Attributes::GetValues(Actor* actor)
{
	std::vector<float> retVal;
	if (!actor)
		return retVal;
	retVal.reserve(allAttributes.size());
	BSWriteLocker lock(&actor->avLock);
	for(const auto attributeId : allAttributes)
	{
		const float val = GetValueCore(actor, attributeId);
		retVal.emplace_back(val);
	}
	return retVal;
}
SInt32 Attributes::GetIndex(UInt32 attributeId)
{
	BSReadLocker lock(&lockObject);
	if (cache.empty())
		return -1;
	const auto ptr = cache.find(attributeId);
	if (ptr == cache.end())
		return -1;
	return ptr->second->Index;
}

bool Attributes::Print(UInt32 attributeId, float value, char* output, UInt32& outColor)
{
	BSReadLocker lock(&lockObject);
	if (cache.empty())
		return false;
	const auto ptr = cache.find(attributeId);
	if (ptr == cache.end())
		return false;
	outColor = ColorCoding::GetColor(attributeId, value);
	return ptr->second->Print(value, output);
}
bool Attributes::PrintIndex(UInt32 index, float value, char* output, UInt32& outColor)
{
	BSReadLocker lock(&lockObject);
	if (cache.empty())
		return false;
	if (index >= allAttributes.size())
		return false;
	const UInt32 attributeId = allAttributes[index];
	const auto ptr = cache.find(attributeId);
	if (ptr == cache.end())
		return false;
	outColor = ColorCoding::GetColor(attributeId, value);
	return ptr->second->Print(value, output);
}

