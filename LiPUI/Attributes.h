#pragma once
#include <unordered_map>

#include "f4se/PapyrusArgs.h"

enum AttributeType
{
	Wrong,
	Integer,
	Float
};

class PrintableValue
{
public:
	UInt32 Index;
	virtual bool Print(float value, char* output) const = 0;
	virtual AttributeType GetType() const = 0;
	virtual ~PrintableValue() = default;
	PrintableValue(UInt32 index)
		:Index(index)
	{
		
	}
	PrintableValue() = delete;
	PrintableValue(const PrintableValue&) = default;
	PrintableValue(PrintableValue&&) = default;
	PrintableValue& operator=(const PrintableValue&) = default;
	PrintableValue& operator=(PrintableValue&&) = default;
};


class Attributes
{
	static BSReadWriteLock lockObject;
	static std::unordered_map<UInt32, std::unique_ptr<PrintableValue>> cache;
	static std::vector<UInt32> allAttributes;
public:
	static constexpr size_t StringLength = 0x10;

	static bool Ready();
	static UInt32 Length();
	static AttributeType GetAttributeType(UInt32 attributeId);
	static bool BuildCache(VMArray<ActorValueInfo*>& attributes, VMArray<bool>& flags);
	static std::vector<float> GetValues(Actor* actor);
	static SInt32 GetIndex(UInt32 attributeId);
	static bool Print(UInt32 attributeId, float value, char* output, UInt32& outColor);
	static bool PrintIndex(UInt32 index, float value, char* output, UInt32& outColor);
};