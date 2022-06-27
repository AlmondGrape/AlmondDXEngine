#pragma once
#include "SingletonBase.h"
#include "JsonDoc.h"
#include "Almond_GameEngine.h"

namespace ProjectN
{
	/// <summary>
	/// WinMain�� ����� ���� ��ü
	/// </summary>
	class WinManager
	{
		DECLARE_SINGLETON_CLASS(WinManager)

	public:
		LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		HRESULT Initialize(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
			_In_ LPTSTR lpCmdLine, _In_ int nCmdShow, std::weak_ptr<JsonDoc> windowsettingDoc);
		INT Loop();

	private:
		ATOM MyRegisterClass();
		BOOL InitInstance();


	private:
		// ���� ����
		HINSTANCE m_hInst = 0;         // ���� �ν��Ͻ��Դϴ�.
		HWND m_hWnd = 0;
		bool m_minimized = false;
		bool m_maximized = false;
		bool m_resizingNow = false;

		std::weak_ptr<JsonDoc> m_pWinSettingDoc;
		std::shared_ptr<Almond::IAlmondEngine> m_pEngineInst;
		
	};

}
