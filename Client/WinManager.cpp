#include "pch.h"
#include "WinManager.h"

using namespace ProjectN;

IMPLEMENT_SINGLETON_CLASS(WinManager);

// extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return WinManager::SingletonInstance.MsgProc(hWnd, uMsg, wParam, lParam);
}

// 윈도우 창 초기화 함수
HRESULT WinManager::Initialize(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow, std::weak_ptr<JsonDoc> windowsettingDoc)
{
	m_hInst = hInstance;
	m_pWinSettingDoc = windowsettingDoc;
		
	m_pEngineInst = Almond::Almond_GameEngine::GetInstance();

	// 윈도 클래스 등록 및, 인스턴스 생성 및 초기화
	MyRegisterClass();
	if (!InitInstance())
	{
		return ERROR;
	}

	return NO_ERROR;

}

LRESULT CALLBACK WinManager::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	HDC			hdc;
	PAINTSTRUCT ps;

	//if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
	//	return true;

	switch (uMsg)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...			
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


INT WinManager::Loop()
{
	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// 여기 씬 루프를 돌리자.
			// SceneManager::SingletonInstance.SceneLoop();
			m_pEngineInst->EngineLoop();
		}
	}

	return (int)msg.wParam;
}

ATOM WinManager::MyRegisterClass()
{
	std::shared_ptr<JsonDoc> _winSetting = m_pWinSettingDoc.lock();
	std::wstring applicationName(_winSetting->GetUnicodeStrByID(NULL, "ApplicationName"));

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = applicationName.c_str();
	wcex.hIconSm = LoadIcon(m_hInst, IDI_APPLICATION);

	return RegisterClassExW(&wcex);
}

BOOL WinManager::InitInstance()
{
	std::shared_ptr<JsonDoc> _winSetting = m_pWinSettingDoc.lock();

	std::wstring applicationName(_winSetting->GetUnicodeStrByID(NULL, "ApplicationName"));
	std::wstring titleName(_winSetting->GetUnicodeStrByID(NULL, "TitleName"));
	bool _IsBorderless = _winSetting->GetNumNBoolByID<bool>(NULL, "BorderessWindow");
	int _windowWidth = _winSetting->GetNumNBoolByID<int>(NULL, "WindowWidth");
	int _windowHeight = _winSetting->GetNumNBoolByID<int>(NULL, "WindowHeight");

	int posX = (GetSystemMetrics(SM_CXSCREEN) - _windowWidth) / 2;
	int posY = (GetSystemMetrics(SM_CYSCREEN) - _windowHeight) / 2;

	if (_IsBorderless == false)
	{
		m_hWnd = CreateWindowW(
			applicationName.c_str(),
			titleName.c_str(),
			WS_OVERLAPPED | WS_SYSMENU,
			posX, posY, _windowWidth, _windowHeight, nullptr, nullptr, m_hInst, nullptr);
	}
	else
	{
		m_hWnd = CreateWindowExW(
			WS_EX_APPWINDOW,
			applicationName.c_str(),
			titleName.c_str(),
			WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
			posX, posY, _windowWidth, _windowHeight, nullptr, nullptr, m_hInst, nullptr);
	}

	if (!m_hWnd)
	{
		return FALSE;
	}

	RECT _rect;
	GetClientRect(m_hWnd, &_rect);


	// 이제 여기서 엔진 인스턴스들을 초기화 한다.
	m_pEngineInst->Initialize(m_hWnd, _windowWidth, _windowHeight);

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	return TRUE;

}
