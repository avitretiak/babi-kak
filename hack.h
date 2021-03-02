#pragma once
#include "csgo.h"
using namespace offsets;

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

#define ABS(x) ((x < 0) ? (-x) : (x))

#define TORAD(x) ((x) * 0.01745329252)

#define W2S(x,y) hack->WorldToScreen(x, y)

struct Vec2 {
	float x, y;
};

struct Vec3 {
	float x, y, z;
};

struct Vec4 {
	float x, y, z, w;
};

struct GlowStruct
{
	union
	{
		DEFINE_MEMBER_N(float, r, 0x4);
		DEFINE_MEMBER_N(float, g, 0x8);
		DEFINE_MEMBER_N(float, b, 0xC);
		DEFINE_MEMBER_N(float, a, 0x10);
		DEFINE_MEMBER_N(bool, occluded, 0x24);
		DEFINE_MEMBER_N(bool, unoccluded, 0x25);
	};
};

class Ent {
public:
	union {
		DEFINE_MEMBER_N(bool, isDormant, m_bDormant);
		DEFINE_MEMBER_N(Vec3, vecOrigin, m_vecOrigin);
		DEFINE_MEMBER_N(Vec3, vecVelocity, m_vecVelocity);


		DEFINE_MEMBER_N(int, iHealth, m_iHealth); 
		DEFINE_MEMBER_N(int, armorValue, m_ArmorValue);
		DEFINE_MEMBER_N(int, iTeamNum, m_iTeamNum);
		DEFINE_MEMBER_N(int, boneMatrix, m_dwBoneMatrix);
		DEFINE_MEMBER_N(float, angEyeAnglesX, m_angEyeAnglesX);
		DEFINE_MEMBER_N(float, angEyeAnglesY, m_angEyeAnglesY);
		DEFINE_MEMBER_N(Vec3, aimPunchAngle, m_aimPunchAngle);
		DEFINE_MEMBER_N(int, flashDuration, m_flFlashDuration);
		DEFINE_MEMBER_N(bool, bHasHelmet, m_bHasHelmet);
		DEFINE_MEMBER_N(bool, Spotted, m_bSpotted);
		DEFINE_MEMBER_N(int, glowIndex, m_iGlowIndex);

	};
};

class EntListObj {
public:
	struct Ent* ent;
	char padding[12];
};

class EntList {
public:
	EntListObj ents[1023];
};



class Hack {
public:
	uintptr_t engine;
	uintptr_t client;
	uintptr_t clientState;
	uintptr_t glowObjMg;
	Ent* localEnt;
	EntList* entList;
	float viewMatrix[16];
	Vec3* viewAngles;
	ID3DXLine* LineL;
	ID3DXFont* FontF;

	Vec2 crosshair2D;
	int crosshairSize = 12;

	~Hack();

	void Init();
	void Update();
	bool CheckValidEnt(Ent* ent);
	bool WorldToScreen(Vec3 pos, Vec2& screen);
	Vec3 GetBonePos(Ent* ent, int bone);
	Vec3 TransformVec(Vec3 src, Vec3 ang, float d);

	int getClassID(Ent* ent);

	struct Settings {
		bool showTeammates = true;
		bool snaplines = false;
		bool box2D = false;
		bool status2D = false;
		bool statusText = true;
		bool box3D = true;
		bool velEsp = true;
		bool no_flash = true;
		bool headlineEsp = true;
		bool rcsCrosshair = true;
		bool bhop = true;
		bool glowHack = true;
		bool radar = true;
		bool rcs = true;
		bool headEsp = true;
		bool infoEsp = true;
	}settings;
};