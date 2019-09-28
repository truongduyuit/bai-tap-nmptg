/* =============================================================
	INTRODUCTION TO GAME PROGRAMMING SE102
	
	SAMPLE 02 - SPRITE AND ANIMATION

	This sample illustrates how to:

		1/ Render a sprite (within a sprite sheet)
		2/ How to manage sprites/animations in a game
		3/ Enhance CGameObject with sprite animation
================================================================ */

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"02 - Sprite animation"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(200, 200, 255)
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 60

//#define ID_TEX_MARIO 0
//#define ID_TEX_ENEMY 10
//#define ID_TEX_MISC 20



CGame *game;
CGameObject *mario;
CGameObject *turtle;

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

/*
	Load all game resources 
	In this example: load textures, sprites, animations and mario object
*/

LPWSTR ConvertString(const string& instr)
{
	int bufferlen = ::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), NULL, 0);

	if (bufferlen == 0)
	{
		return 0;
	}

	LPWSTR widestr = new WCHAR[bufferlen + 1];

	::MultiByteToWideChar(CP_ACP, 0, instr.c_str(), instr.size(), widestr, bufferlen);

	widestr[bufferlen] = 0;
	return widestr;
}

void LoadResources()
{
	CTextures * textures = CTextures::GetInstance();
	ifstream ifs("textures.txt", ios::in);

	if (ifs.fail())
	{
		DebugOut(L"[ERROR] Input textures is failed !");
		return;
	}

	while (!ifs.eof())
	{
		string id_texture, path_texture, r, g, b;

		getline(ifs, id_texture);
		getline(ifs, path_texture);
		getline(ifs, r);
		getline(ifs, g);
		getline(ifs, b);

		textures->Add(stoi(id_texture), ConvertString(path_texture), D3DCOLOR_XRGB(stoi(r), stoi(g), stoi(b)));
	}



	ifs.close();

	//textures->Add(ID_TEX_MARIO, L"textures\\mario.png", D3DCOLOR_XRGB(176, 224, 248));		// load textture mario

	//textures->Add(ID_TEX_ENEMY, L"textures\\enemies.png", D3DCOLOR_XRGB(156, 219, 239));

	//textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(156, 219, 239));

	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance(); 
	
	
	ifstream ifss("position_sprite.txt", ios::in);
	if (ifss.fail())
	{
		DebugOut(L"[ERROR] Input position sprite is failed !");
		return;
	}

	while (!ifss.eof())
	{
		string id_texture, n, id_ani;
		getline(ifss, id_texture);
		getline(ifss, n);
		getline(ifss, id_ani);

		LPANIMATION ani;
		ani = new CAnimation(100);

		for (int i = 0; i < stoi(n); i++)
		{
			string id, l, t, r, b;
			getline(ifss, id);
			getline(ifss, l);
			getline(ifss, t);
			getline(ifss, r);
			getline(ifss, b);

			LPDIRECT3DTEXTURE9 tex = textures->Get(stoi(id_texture));
			sprites->Add(stoi(id), stoi(l), stoi(t), stoi(r), stoi(b), tex);

			
			ani->Add(stoi(id));
		}

		animations->Add(stoi(id_ani), ani);
	}

	ifss.close();

	ifstream ifsss("positionANDani_textures.txt", ios::in);
	if (ifsss.fail())
	{
		DebugOut(L"[ERROR] Input position and animation of texture is failed !");
		return;
	}

	while (!ifsss.eof())
	{

		string obj, n, p_x, p_y;
		getline(ifsss, obj);
		getline(ifsss, n);

		if (obj == "mario")
		{
			mario = new CGameObject();

			for (int i = 0; i < stoi(n); i++)
			{
				string id_ani;
				getline(ifsss, id_ani);
				mario->AddAnimation(stoi(id_ani));
			}

			getline(ifsss, p_x);
			getline(ifsss, p_y);

			mario->SetPosition(stof(p_x), stof(p_y));
		}
		else if (obj == "turtle")
		{
			turtle = new CGameObject();
			for (int i = 0; i < stoi(n); i++)
			{
				string id_ani;
				getline(ifsss, id_ani);

				turtle->AddAnimation(stoi(id_ani));
			}

			getline(ifsss, p_x);
			getline(ifsss, p_y);

			turtle->SetPosition(stof(p_x), stof(p_y));
		}

	}

	ifsss.close();

	//mario = new CGameObject();
	//mario->AddAnimation(500);
	//mario->AddAnimation(501);
	////mario->AddAnimation(510);

	//turtle = new CGameObject();
	//turtle->AddAnimation(1001);
	//turtle->AddAnimation(1002);

	//mario->SetPosition(10.0f, 100.0f);
	//turtle->SetPosition(5.0f, 30.0f);

	//LPDIRECT3DTEXTURE9 texMario = textures->Get(ID_TEX_MARIO);
	// readline => id, left, top, right 

	//sprites->Add(10001, 246, 154, 259, 181, texMario);
	//sprites->Add(10002, 275, 154, 290, 181, texMario);
	//sprites->Add(10003, 304, 154, 321, 181, texMario);

	//sprites->Add(10011, 186, 154, 199, 181, texMario);
	//sprites->Add(10012, 155, 154, 170, 181, texMario);
	//sprites->Add(10013, 125, 154, 140, 181, texMario);


	//LPDIRECT3DTEXTURE9 texEnermy = textures->Get(ID_TEX_ENEMY);
	//sprites->Add(20001, 208, 145, 235, 185, texEnermy);
	//sprites->Add(20002, 238, 145, 265, 185, texEnermy);
	//sprites->Add(20003, 263, 145, 290, 185, texEnermy);
	//sprites->Add(20004, 288, 145, 310, 185, texEnermy);
	


	//LPANIMATION ani;

	//ani = new CAnimation(100);
	//ani->Add(10001);
	//ani->Add(10002);
	//ani->Add(10003);
	//animations->Add(500, ani);

	//ani = new CAnimation(100);
	//ani->Add(10011);
	//ani->Add(10012);
	//ani->Add(10013);
	//animations->Add(501, ani);

	//ani = new CAnimation(100);
	//ani->Add(20001);
	//ani->Add(20002);
	//ani->Add(20003);
	//ani->Add(20004);
	//animations->Add(1001, ani);

	//ani = new CAnimation(100);
	//ani->Add(20001);
	//ani->Add(20002);
	//ani->Add(20003);
	//ani->Add(20004);
	//animations->Add(1002, ani);

	/*
	ani = new CAnimation(100);
	ani->Add(20001,1000);
	ani->Add(20002);
	ani->Add(20003);
	ani->Add(20004);
	animations->Add(510, ani);
	*/
	
	//mario = new CGameObject();
	//mario->AddAnimation(500);
	//mario->AddAnimation(501);
	////mario->AddAnimation(510);

	//turtle = new CGameObject();
	//turtle->AddAnimation(1001);
	//turtle->AddAnimation(1002);

	//mario->SetPosition(10.0f, 100.0f);
	//turtle->SetPosition(5.0f, 30.0f);
}

/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	mario->Update(dt);
	turtle->Update(dt);
}

/*
	Render a frame 
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		mario->Render();
		turtle->Render();
		//
		// TEST SPRITE DRAW
		//

		/*
		CTextures *textures = CTextures::GetInstance();

		D3DXVECTOR3 p(20, 20, 0);
		RECT r;
		r.left = 274;
		r.top = 234;
		r.right = 292;
		r.bottom = 264;
		spriteHandler->Draw(textures->Get(ID_TEX_MARIO), &r, NULL, &p, D3DCOLOR_XRGB(255, 255, 255));
		*/

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd) 
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;
			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);	
	}

	return 1;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	LoadResources();
	Run();

	return 0;
}