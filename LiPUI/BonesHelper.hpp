#pragma once
#include <vector>

#include "f4se/GameForms.h"
#include "f4se/NiNodes.h"
#include "f4se/GameReferences.h"


// human
const std::vector<const char*> human = 
{
	"Root",
	"COM_Override",
	"COM",
	"SPINE1",
	"SPINE2",
	"Chest",
	"Neck",
	"Head",
};

// [/Root/COM/SPINE1/SPINE2/Chest/Neck/Head]
const std::vector<const char*> generic =
{
	"Root",
	"COM",
	"SPINE1",
	"SPINE2",
	"Chest",
	"Neck",
	"Head",
};

// nearest found - /Root/COM/SPINE1/SPINE2/Chest/RArm_Collarbone
// should have compensation
const std::vector<const char*> supermutant = 
{
	"Root",
	"COM",
	"SPINE1",
	"SPINE2",
	"Chest",
	"RArm_Collarbone",
};

void ListBonesTree(const NiNode * parent, const std::string& prefix)
{
	if (!parent)
		return;
	for (UInt16 i = 0; i < parent->m_children.m_size; i++)
	{
		NiAVObject* child = parent->m_children.m_data[i];
		if (child)
		{
			std::string curName = prefix + "/" + child->m_name.c_str();
			_MESSAGE("[%s]:{%f, %f, %f}", curName.c_str(), child->m_worldTransform.pos.x, child->m_worldTransform.pos.y, child->m_worldTransform.pos.z);
			const NiNode* container = child->GetAsNiNode();
			if (container)
				ListBonesTree(container, curName);
		}
	}
}


NiAVObject* GetNested(NiNode * parent, const std::vector<const char*>& path)
{
	if (!parent)
		return nullptr;
	NiAVObject* current = parent;
	for (const char* section : path)
	{
		if (!current)
			return nullptr;
		BSFixedString name(section);
		current = current->GetObjectByName(&name);
	}
	return current;
}

NiAVObject * FindHead(TESRace* race, NiNode * parent)
{
	if (!race)
		return nullptr;

	if (race->formID == 0x00013794)
	{
		NiAVObject * retVal = GetNested(parent, human);
		if (retVal)
			return retVal;
		return GetNested(parent, generic);
	}
	if (race->formID == 0x0006D7B6)
		return GetNested(parent, supermutant);

	return GetNested(parent, generic);
}

// TODO: optimize
void FindHeadPosition(Actor* a, NiPoint3& headPos, NiPoint3& actorPos)
{
	NiAVObject* head = nullptr;
	NiAVObject * rootNode = a->GetObjectRootNode();
	if (rootNode)
	{
		actorPos.x = rootNode->m_worldTransform.pos.x;
		actorPos.y = rootNode->m_worldTransform.pos.y;
		actorPos.z = rootNode->m_worldTransform.pos.z;
		head = FindHead(a->GetActorRace(), rootNode->GetAsNiNode());
	}
	if (head)
	{
		headPos.x = head->m_worldTransform.pos.x;
		headPos.y = head->m_worldTransform.pos.y;
		headPos.z = head->m_worldTransform.pos.z;
	}
	else
	{
		_MESSAGE("Head not found");
		a->GetMarkerPosition(&headPos);
		if (rootNode)
		{
			//ListBonesTree(rootNode->GetAsNiNode(),"");
			headPos.x = rootNode->m_worldTransform.pos.x;
			headPos.y = rootNode->m_worldTransform.pos.y;
		}
		actorPos.x = headPos.x;
		actorPos.y = headPos.y;
		actorPos.z = headPos.z;
	}
}