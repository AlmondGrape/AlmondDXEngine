// 애플리케이션에 대한 진입점을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "WinManager.h"
#include "JsonDoc.h"

using namespace ProjectN;

#include "Almond_GameEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.

	std::shared_ptr<JsonDoc> winsettingDoc = std::make_shared<JsonDoc>();	

	std::string path = "./contents/windowsetting.json";
	winsettingDoc->Read(path);

	if (SUCCEEDED(WinManager::SingletonInstance.Initialize(hInstance, hPrevInstance, lpCmdLine, nCmdShow, winsettingDoc)))
	{
		// 임시로 씬 만들고..
		//std::shared_ptr<Almond::Scene> _scene = std::make_shared<TestScene>(1);

		//Almond::SceneManager::SingletonInstance.AddScene(_scene);
		//Almond::SceneManager::SingletonInstance.LoadScene(1);

		return WinManager::SingletonInstance.Loop();
	}

	return 0;
}