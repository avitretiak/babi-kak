#include "includes.h"

extern Hack* hack;

void DrawFilledRect(int x, int y, int w, int h, D3DCOLOR color) {
	D3DRECT rect = { x,y,x + w,y + h };
	pDevice->Clear(1, &rect, D3DCLEAR_TARGET, color, 0, 0);
}

void DrawLine(int x1, int y1, int x2, int y2, int thickness, D3DCOLOR color) {
	ID3DXLine* LineL;
	

	if (!hack->LineL)
		D3DXCreateLine(pDevice, &hack->LineL);

	D3DXVECTOR2 Line[2];

	Line[0] = D3DXVECTOR2(x1, y1);
	Line[1] = D3DXVECTOR2(x2, y2);
	hack->LineL->SetWidth(thickness);
	hack->LineL->Draw(Line, 2, color);
}

void DrawLine(Vec2 src, Vec2 dst, int thickness, D3DCOLOR color) {
	DrawLine(src.x, src.y, dst.x, dst.y, thickness, color);
}

void DrawEspBox2D(Vec2 top, Vec2 bot, int thickness, D3DCOLOR color) {
	int height = ABS(top.y - bot.y);
	Vec2 tleft, tright;
	tleft.x = top.x - height / 4;
	tright.x = top.x + height / 4;
	tleft.y = tright.y = top.y;

	Vec2 bleft, bright;
	bleft.x = bot.x - height / 4;
	bright.x = bot.x + height / 4;
	bleft.y = bright.y = bot.y;


	DrawLine(tleft, tright, thickness, color);
	DrawLine(bleft, bright, thickness, color);
	DrawLine(tleft, bleft, thickness, color);
	DrawLine(tright, bright, thickness, color);
}

void DrawEspBox3D(Vec3 top, Vec3 bot, float a, int width, int thickness, D3DCOLOR color) {
	int height3D = ABS(top.z - bot.z);
	Vec3 b1, b2, b3, b4, t1, t2, t3, t4;
	b1.z = b2.z = b3.z = b4.z = bot.z;

	b1.x = bot.x + (cos(TORAD(a + 45)) * width);
	b1.y = bot.y + (sin(TORAD(a + 45)) * width);

	b2.x = bot.x + (cos(TORAD(a + 135)) * width);
	b2.y = bot.y + (sin(TORAD(a + 135)) * width);

	b3.x = bot.x + (cos(TORAD(a + 225)) * width);
	b3.y = bot.y + (sin(TORAD(a + 225)) * width);

	b4.x = bot.x + (cos(TORAD(a + 315)) * width);
	b4.y = bot.y + (sin(TORAD(a + 315)) * width);

	t1.x = b1.x;
	t1.y = b1.y;
	t1.z = b1.z + height3D;

	t2.x = b2.x;
	t2.y = b2.y;
	t2.z = b2.z + height3D;

	t3.x = b3.x;
	t3.y = b3.y;
	t3.z = b3.z + height3D;

	t4.x = b4.x;
	t4.y = b4.y;
	t4.z = b4.z + height3D;

	Vec2 b1_2d, b2_2d, b3_2d, b4_2d, t1_2d, t2_2d, t3_2d, t4_2d;
	if (W2S(b1, b1_2d) && W2S(b2, b2_2d) && W2S(b3, b3_2d) && W2S(b4, b4_2d)
		&& W2S(t1, t1_2d) && W2S(t2, t2_2d) && W2S(t3, t3_2d) && W2S(t4, t4_2d)) {
		
		//columns
		DrawLine(t1_2d, b1_2d, thickness, color);
		DrawLine(t2_2d, b2_2d, thickness, color);
		DrawLine(t3_2d, b3_2d, thickness, color);
		DrawLine(t4_2d, b4_2d, thickness, color);

		// top base
		DrawLine(t1_2d, t2_2d, thickness, color);
		DrawLine(t2_2d, t3_2d, thickness, color);
		DrawLine(t3_2d, t4_2d, thickness, color);
		DrawLine(t4_2d, t1_2d, thickness, color);

		// bot base
		DrawLine(b1_2d, b2_2d, thickness, color);
		DrawLine(b2_2d, b3_2d, thickness, color);
		DrawLine(b3_2d, b4_2d, thickness, color);
		DrawLine(b4_2d, b1_2d, thickness, color);
	}
}

void DrawText(const char* text, float x, float y, D3DCOLOR color) {
	RECT rect;

	if (!hack->FontF)
		D3DXCreateFont(pDevice, 14, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &hack->FontF);

	SetRect(&rect, x + 1, y + 1, x + 1, y + 1);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, D3DCOLOR_ARGB(255, 0, 0, 0));

	SetRect(&rect, x, y, x, y);
	hack->FontF->DrawTextA(NULL, text, -1, &rect, DT_CENTER | DT_NOCLIP, color);
}
