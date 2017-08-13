#pragma once

class GameApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	int Loop();

	HRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	bool InitWindow();
	bool InitDirect3D(BOOL windowed);

	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

	int m_windowWidth;
	int m_windowHeight;
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
};