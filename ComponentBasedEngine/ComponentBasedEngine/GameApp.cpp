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
	if (InitDirect3D(TRUE) == false)
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

bool GameApp::InitDirect3D(BOOL windowed)
{
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	UINT adapterNum = -1;
	//내꺼에서는 하나밖에 안뜨네...(내장 아니면 외장)
	UINT nAdapters = m_pD3D->GetAdapterCount();
	if (nAdapters <= 0) {
		LOG_ERROR(Logging::CHANNEL_CORE, "사용가능한 어댑터가 없습니다.");
		return false;
	}

	std::vector<D3DADAPTER_IDENTIFIER9> adapterIdentifiers(nAdapters);
	for (int i = 0; i < nAdapters; ++i) {
		m_pD3D->GetAdapterIdentifier(i, 0, &adapterIdentifiers[i]);
		LOG_INFO(Logging::CHANNEL_CORE, "Adapter %d : %s", i ,adapterIdentifiers[i].DeviceName);
		LOG_INFO(Logging::CHANNEL_CORE, "Adapter %d Desc : %s", i, adapterIdentifiers[i].Description);
	}
	

	adapterNum = 0;

	std::vector<D3DFORMAT> fmtArr = { D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5,D3DFMT_X8R8G8B8, D3DFMT_X1R5G5B5, D3DFMT_R5G6B5 };
	std::vector<D3DDEVTYPE> devArr = { D3DDEVTYPE_HAL ,D3DDEVTYPE_HAL ,D3DDEVTYPE_HAL,D3DDEVTYPE_REF,D3DDEVTYPE_REF,D3DDEVTYPE_REF };

	size_t iConfig;
	for (iConfig = 0; iConfig < fmtArr.size(); ++iConfig) {
		if (SUCCEEDED(m_pD3D->CheckDeviceType(adapterNum, devArr[iConfig], fmtArr[iConfig], fmtArr[iConfig], windowed))) {
			break;
		}
	}

	if (iConfig >= fmtArr.size()) {
		LOG_ERROR(Logging::CHANNEL_CORE, "적절한 디바이스를 찾지 못했습니다.");
		return false;
	}

	switch (devArr[iConfig]) {
	case D3DDEVTYPE_HAL:
		LOG_INFO(Logging::CHANNEL_CORE, "DeviceType : HAL");
		break;
	case D3DDEVTYPE_REF:
		LOG_INFO(Logging::CHANNEL_CORE, "DeviceType : REF");
		break;
	}

	switch (fmtArr[iConfig]) {
	case D3DFMT_X8R8G8B8:
		LOG_INFO(Logging::CHANNEL_CORE, "AdapterFormat : X8R8G8B8");
		break;
	case D3DFMT_X1R5G5B5:
		LOG_INFO(Logging::CHANNEL_CORE, "AdapterFormat : X1R5G5B5");
		break;
	case D3DFMT_R5G6B5:
		LOG_INFO(Logging::CHANNEL_CORE, "AdapterFormat : R5G6B5");
		break;
	}

	D3DCAPS9 stCaps;
	int nVertexProcessing;

	m_pD3D->GetDeviceCaps(adapterNum, devArr[iConfig], &stCaps);

	if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		LOG_INFO(Logging::CHANNEL_CORE, "Vertex Processing : Hardware");
		nVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
#ifdef NDEBUG
		if (stCaps.DevCaps & D3DDEVCAPS_PUREDEVICE) {
			//속도는 빠르지만 디버깅할 떄 안좋음.
			LOG_INFO(Logging::CHANNEL_CORE, "Enable PureDevice");
			nVertexProcessing |= D3DCREATE_PUREDEVICE;
		}
#endif
	}
	else {
		LOG_INFO(Logging::CHANNEL_CORE, "Vertex Processing : Software");
		nVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	//TODO : 이게 뭐지?
	int presentInterval = D3DPRESENT_INTERVAL_DEFAULT;
	//if (stCaps.PresentationIntervals & D3DPRESENT_INTERVAL_FOUR) {
	//	presentInterval = D3DPRESENT_INTERVAL_FOUR;
	//}

	//깊이-스텐실 버퍼 포맷 
	D3DFORMAT depthstencil = D3DFMT_D24S8;
	if (FAILED(m_pD3D->CheckDeviceFormat(adapterNum, devArr[iConfig], fmtArr[iConfig], D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depthstencil))) {
		//게임 구현에 필요한 Depth-Stencil버퍼를 이요할 수 없기 때문에 게임을 실행할 수 없는건가?
		LOG_ERROR(Logging::CHANNEL_CORE, "Depth-Stencil format D24S8 unsupported.");
		return false;
	}

	//멀티 샘플링
	D3DMULTISAMPLE_TYPE multiSampleType = D3DMULTISAMPLE_NONE;
	DWORD maxQuality = 0;
	for (int i = D3DMULTISAMPLE_2_SAMPLES; i < D3DMULTISAMPLE_16_SAMPLES; ++i) {
		D3DMULTISAMPLE_TYPE mst = (D3DMULTISAMPLE_TYPE)i;
		DWORD quality;
		if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(adapterNum, devArr[iConfig], fmtArr[iConfig], windowed, mst, &quality))) {
			if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(adapterNum, devArr[iConfig], depthstencil, windowed, mst, &quality))) {
				LOG_INFO(Logging::CHANNEL_CORE, "%d Sample Supported", mst);
				multiSampleType = mst;
				maxQuality = quality - 1;
			}
		}
	}

	//
	D3DDISPLAYMODE bestmode;
	UINT nAdapterMode = m_pD3D->GetAdapterModeCount(adapterNum, fmtArr[iConfig]);
	assert(nAdapterMode > 0);
	m_pD3D->EnumAdapterModes(adapterNum, fmtArr[iConfig], 0, &bestmode);
	for (UINT i = 1; i < nAdapterMode; i++)
	{
		D3DDISPLAYMODE dispmode;
		m_pD3D->EnumAdapterModes(adapterNum, fmtArr[iConfig], i, &dispmode);
		if (dispmode.Width > bestmode.Width)
		{
			bestmode.Width = dispmode.Width;
			bestmode.Height = dispmode.Height;
			bestmode.RefreshRate = dispmode.RefreshRate;
			continue;
		}
		if (dispmode.Height > bestmode.Height)
		{
			bestmode.Height = dispmode.Height;
			bestmode.RefreshRate = dispmode.RefreshRate;
			continue;
		}
		if (dispmode.RefreshRate > bestmode.RefreshRate)
		{
			bestmode.RefreshRate = dispmode.RefreshRate;
			continue;
		}
	}
	LOG_INFO(Logging::CHANNEL_CORE, "Width X Height : %u X %u", bestmode.Width, bestmode.Height);
	LOG_INFO(Logging::CHANNEL_CORE, "RefreshRate : %u", bestmode.RefreshRate);

	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;
	stD3DPP.Windowed = windowed;
	stD3DPP.BackBufferFormat = fmtArr[iConfig];
	stD3DPP.EnableAutoDepthStencil = TRUE;
	stD3DPP.AutoDepthStencilFormat = depthstencil;
	stD3DPP.PresentationInterval = presentInterval;
	stD3DPP.MultiSampleType = multiSampleType;
	stD3DPP.MultiSampleQuality = maxQuality;
	if (!windowed) {
		stD3DPP.BackBufferHeight = bestmode.Height;
		stD3DPP.BackBufferWidth = bestmode.Width;
		stD3DPP.FullScreen_RefreshRateInHz = bestmode.RefreshRate;
	}

	HRESULT ret = m_pD3D->CreateDevice(adapterNum,
		devArr[iConfig],
		m_hWnd,
		nVertexProcessing,
		&stD3DPP,
		&m_pD3DDevice);
	if (FAILED(ret)) {
		LOG_ERROR(Logging::CHANNEL_CORE, "CreateDevice Error : %s", DXGetErrorDescriptionA(ret));
		return false;
	}

	return true;
}
