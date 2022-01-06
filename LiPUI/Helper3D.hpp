#pragma once
#include "f4se/NiObjects.h"
#include "f4se/GameCamera.h"

//constexpr float PI = 3.14159265358979323846;

struct Point2D
{
	SInt32 X;
	SInt32 Y;
};
struct Point3D
{
	Point3D() = default;
	Point3D(const NiPoint3& p)
	{
		X = p.x;
		Y = p.y;
		Z = p.z;
	}
	Point3D(float x, float y, float z)
	{
		X = x;
		Y = y;
		Z = z;
	}
	float X;
	float Y;
	float Z;

	float Length2() const
	{
		return X*X + Y*Y + Z*Z;
	}
	void Normalize()
	{
		const float len = sqrt(Length2());
		if (len < 0.000001)
		{
			X = 0;
			Y = 0;
			Z = 0;
		}
		else
		{
			X /= len;
			Y /= len;
			Z /= len;
		}
	}
	float Dot(const Point3D& p) const
	{
		return X * p.X + Y * p.Y +  Z * p.Z;
	}
};

struct Quaternion
{
	Quaternion() = default;
	Quaternion(const NiQuaternion& q)
	{
		X = q.m_fX;
		Y = q.m_fY;
		Z = q.m_fZ;
		W = q.m_fW;
	}
	float X;
	float Y;
	float Z;
	float W;

	Point3D Forward() const // magic %). see discussion on https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/
	{
		Point3D retVal;
		retVal.X = 2 * (X*Y - W*Z);
		retVal.Y = 1 - 2 * (X*X + Z*Z);
		retVal.Z = 2 * (Y*Z + W*X);
		return retVal;
	}
};

constexpr float posDelta = 0.001f;
constexpr float rotDelta = 0.0001f;
struct PlayerCameraDetails
{
	Quaternion Rotation;
	Point3D Position;
	UInt32 Mode;
	bool operator != (const PlayerCameraDetails& other) const noexcept
	{
		return
			std::fabsf(Rotation.W - other.Rotation.W) > rotDelta ||
			std::fabsf(Rotation.X - other.Rotation.X) > rotDelta ||
			std::fabsf(Rotation.Y - other.Rotation.Y) > rotDelta ||
			std::fabsf(Rotation.Z - other.Rotation.Z) > rotDelta ||
			std::fabsf(Position.X - other.Position.X) > posDelta ||
			std::fabsf(Position.Y - other.Position.Y) > posDelta ||
			std::fabsf(Position.Z - other.Position.Z) > posDelta;

	}
	void Print() const
	{
		_MESSAGE("r={%f, %f, %f, %f}, p={%f, %f, %f}", Rotation.X, Rotation.Y, Rotation.Z, Rotation.W, Position.X, Position.Y, Position.Z);
	}
};


