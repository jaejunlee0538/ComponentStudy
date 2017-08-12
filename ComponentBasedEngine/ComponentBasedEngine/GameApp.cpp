#include "stdafx.h"
#include "GameApp.h"
#include <Windows.h>
#include "TimeManager.h"
#include "FreqEstimater.h"
namespace {
	GameApp* app = nullptr;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return app->MsgProc(hWnd, message, wParam, lParam);
}

GameApp::GameApp(HINSTANCE hInstance)
	:m_hInstance(hInstance)
{
	app = this;
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (InitWindow() == false)
		return false;
	if (InitDirect3D() == false)
		return false;
	return true;
}

int GameApp::Loop()
{
	MSG msg;
	FreqEstimater freq(1.0f);
	wchar_t buffer[256];
	while (true) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else {
			TimeManager::GetSingleton().Update();
			freq.update(TimeManager::GetSingleton().CurrentRealTime());
			//
			swprintf_s(buffer, 256, L"FSP:%.1lf Time:%.2lf Real-Time:%.2lf", freq.getFPS(), TimeManager::GetSingleton().CurrentTime(), TimeManager::GetSingleton().CurrentRealTime());
			SetWindowTextW(m_hWnd, buffer);

			Logging::Logger::GetSingleton().Log(Logging::CHANNEL_GENERAL, Logging::LEVEL_VERBOSE, "%.1f FPS", freq.getFPS());
			//
		}
	}
	return 0;
}

HRESULT GameApp::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool GameApp::InitWindow()
{
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hInstance = m_hInstance;
	wndClass.lpfnWndProc = (WNDPROC)WndProc;
	wndClass.lpszClassName = L"GameApp";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wndClass);


	HWND hWnd = CreateWindowW(L"GameApp", L"GameApp", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, m_hInstance, nullptr);

	if (!hWnd) {
		return false;
	}
	m_hWnd = hWnd;
	RECT rect = { 0, 0, 800, 600 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(m_hWnd, NULL, 50, 50, rect.right - rect.left,
		rect.bottom - rect.top, SWP_NOZORDER | SWP_NOMOVE);


	ShowWindow(hWnd, SW_SHOW);
	return true;
}

bool GameApp::InitDirect3D()
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DCAPS9 stCaps;
	int nVertexProcessing;
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps);
	if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else {
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed = TRUE;
	stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D16;

	m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		m_hWnd,
		nVertexProcessing,
		&stD3DPP,
		&m_pD3DDevice);
	return true;
}
