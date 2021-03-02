#include "includes.h"
#include <sstream>
#include <string.h>
using namespace offsets;

// data
void* d3d9Device[119];
BYTE EndSceneBytes[7]{ 0 };
tEndScene oEndScene = nullptr;
extern LPDIRECT3DDEVICE9 pDevice = nullptr;
Hack* hack;
void AntiFlash();
void GlowHack(Ent* ent);
void RadarHack(Ent* ent);
void EspSnapLines(Vec3 origin, Vec2 pos2D, D3DCOLOR color);
void HeadEsp(Ent* ent, int thickness, D3DCOLOR color);
void InfoEsp(Ent* ent, Vec2 pos2D);
float GlowEnemyColor[4] = { 1.f, 1.f, 0.f, 1.f };
float GlowTeamColor[4] = { 0.f, 0.5f, 1.f, 1.f };

bool imguiInitialized;
bool g_ShowMenu = true;


// hook function
void APIENTRY hkEndScene(LPDIRECT3DDEVICE9 o_pDevice) {
	if (!pDevice) 
		pDevice = o_pDevice;

	/*for (int j = 65; j < 1022; j++) {
		Ent* curEnt = hack->entList->ents[j].ent;
		if (curEnt == nullptr || curEnt->isDormant)
			continue;

		int nClassID = hack->getClassID(curEnt);

		Vec3 originVector = curEnt->vecOrigin;
		originVector.z += 8;

		DrawEspBox3D(originVector, curEnt->vecOrigin, 0, 25, 2, D3DCOLOR_ARGB(255, 0, 255, 0));

	}*/

	// drawing stuff
	DrawText("EL ABI KAKER", windowWidth / 2, windowHeight - 20, D3DCOLOR_ARGB(255, 255, 255, 255));
	for (int i = 1; i < 64; i++) {

		Ent* curEnt = hack->entList->ents[i].ent;

		if (!hack->CheckValidEnt(curEnt))
			continue;
		if (!hack->settings.showTeammates && (curEnt->iTeamNum == hack->localEnt->iTeamNum))
			continue;

		GlowHack(curEnt);
		RadarHack(curEnt);

		D3DCOLOR color;
		if (curEnt->iTeamNum == hack->localEnt->iTeamNum)
			color = D3DCOLOR_ARGB(255, 0, 255, 0);
		else
			color = D3DCOLOR_ARGB(255, 255, 0, 0);

		

		Vec3 entHead3D = hack->GetBonePos(curEnt, 8);
		entHead3D.z += 8;
		Vec2 entPos2D, entHead2D;
		if (hack->WorldToScreen(curEnt->vecOrigin, entPos2D)) {
			EspSnapLines(curEnt->vecOrigin, entPos2D, color);
			InfoEsp(curEnt, entPos2D);


			if (hack->WorldToScreen(entHead3D, entHead2D)) {

				HeadEsp(curEnt, 2, color);
				if (hack->settings.box2D)
					DrawEspBox2D(entPos2D, entHead2D, 2, color);

				if (hack->settings.box3D)
					DrawEspBox3D(entHead3D, curEnt->vecOrigin, curEnt->angEyeAnglesY, 25, 2, color);

				if (hack->settings.status2D) {
					int height = ABS(entPos2D.y - entHead2D.y);
					int dX = (entPos2D.x - entHead2D.x);

					float healthPerc = curEnt->iHealth / 100.f;
					float armorPerc = curEnt->armorValue / 100.f;

					Vec2 botHealth, topHealth, botArmor, topArmor;

					int healthHeight = height * healthPerc;
					int armorHeight = height * armorPerc;

					botHealth.y = botArmor.y = entPos2D.y;

					botHealth.x = entPos2D.x - (height / 4) - 2;
					botArmor.x = entPos2D.x + (height / 4) + 2;

					topHealth.y = entHead2D.y + height - healthHeight;
					topArmor.y = entHead2D.y + height - armorHeight;

					topHealth.x = entPos2D.x - (height / 4) - 2 - (dX * healthPerc);
					topArmor.x = entPos2D.x + (height / 4) + 2 - (dX * armorPerc);

					DrawLine(botHealth, topHealth, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
					DrawLine(botArmor, topArmor, 5, D3DCOLOR_ARGB(255, 0, 0, 255));
				}
			}
		}
	}

	if (hack->settings.rcsCrosshair) {
		Vec2 left, right, top, bot;
		left = right = top = bot = hack->crosshair2D;
		left.x -= hack->crosshairSize;
		right.x += hack->crosshairSize;
		top.y -= hack->crosshairSize;
		bot.y += hack->crosshairSize;

		DrawLine(left, right, 4, D3DCOLOR_ARGB(60, 0, 255, 0));
		DrawLine(top, bot, 4, D3DCOLOR_ARGB(60, 0, 255, 0));
	}
	oEndScene(pDevice);
}

DWORD WINAPI HackThread(HMODULE hModule) {

	// hook
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device))) {
		memcpy(EndSceneBytes, (char*)d3d9Device[42], 7);
		oEndScene = (tEndScene)TrampHook((char*)d3d9Device[42], (char*)hkEndScene, 7);
	}

	hack = new Hack();
	hack->Init();

	while (!GetAsyncKeyState(VK_END)) {
		hack->Update();
		AntiFlash();

		Vec3 pAng = hack->localEnt->aimPunchAngle;
		hack->crosshair2D.x = windowWidth / 2 - (windowWidth / 90 * pAng.y);
		hack->crosshair2D.y = windowHeight / 2 + (windowHeight / 90 * pAng.x);
	}

	//unhook
	Patch((BYTE*)d3d9Device[42], EndSceneBytes, 7);

	Sleep(1000);

	// uninject
	FreeLibraryAndExitThread(hModule, 0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpr) {
	if (reason == DLL_PROCESS_ATTACH)
		CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, 0));
	return TRUE;
}

void AntiFlash() {
	if (hack->settings.no_flash && hack->localEnt->flashDuration > 0)
		hack->localEnt->flashDuration = 0;
}

void GlowHack(Ent* ent)
{
	if (hack->settings.glowHack && !ent->isDormant && ent->iHealth > 0 && ent->iTeamNum != 0)
	{
		GlowStruct* pGlow = (GlowStruct*)(hack->glowObjMg + (ent->glowIndex * 0x38));

		if (ent->iTeamNum == hack->localEnt->iTeamNum)
		{
			pGlow->r = GlowTeamColor[0];
			pGlow->g = GlowTeamColor[1];
			pGlow->b = GlowTeamColor[2];
			pGlow->a = GlowTeamColor[3];
		}

		if (ent->iTeamNum != hack->localEnt->iTeamNum)
		{
			pGlow->r = GlowEnemyColor[0];
			pGlow->g = GlowEnemyColor[1];
			pGlow->b = GlowEnemyColor[2];
			pGlow->a = GlowEnemyColor[3];
		}

		pGlow->occluded = true;
		pGlow->unoccluded = false;
	}
}

void RadarHack(Ent* ent)
{
	if (hack->settings.radar && !ent->isDormant && !ent->Spotted)
	{
		ent->Spotted = true;
	}
}

void EspSnapLines(Vec3 origin, Vec2 pos2D, D3DCOLOR color) {
	if (hack->settings.snaplines)
		DrawLine(pos2D.x, pos2D.y, windowWidth / 2, windowHeight, 2, color);
}

void HeadEsp(Ent* ent, int thickness, D3DCOLOR color) {
	if (hack->settings.headEsp) {
		Vec3 head3D = hack->GetBonePos(ent, 8);
		Vec3 entAngles;
		entAngles.x = ent->angEyeAnglesX;
		entAngles.y = ent->angEyeAnglesY;
		entAngles.z = 0;
		Vec3 endpoint = hack->TransformVec(head3D, entAngles, 60);
		Vec2 endPoint2D, head2D;
		hack->WorldToScreen(head3D, head2D);
		if (hack->WorldToScreen(endpoint, endPoint2D)) {
			DrawLine(head2D, endPoint2D, thickness, color);
		}
	}
}

void InfoEsp(Ent* ent, Vec2 pos2D) {
	if (hack->settings.infoEsp) {
		std::stringstream s1, s2;
		s1 << ent->iHealth;
		s2 << ent->armorValue;

		std::string t1 = "HP: " + s1.str();
		std::string t2 = "AP: " + s2.str();

		char* healthMsg = (char*)t1.c_str();
		char* armorMsg = (char*)t2.c_str();

		DrawText(healthMsg, pos2D.x, pos2D.y, D3DCOLOR_ARGB(255, 255, 255, 255));
		DrawText(armorMsg, pos2D.x, pos2D.y + 16, D3DCOLOR_ARGB(255, 255, 255, 255));

		if (!ent->bHasHelmet)
			DrawText("sin casquito :)", pos2D.x, pos2D.y + 32, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}
