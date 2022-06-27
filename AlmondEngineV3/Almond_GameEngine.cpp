#include "pch.h"
#include "Almond_GameEngine.h"

using namespace Almond;

void Almond_GameEngine::Initialize(HWND hWnd, int width, int height)
{	
		
	IGraphics::GetInstance()->Initialize(hWnd, width, height);


}

void Almond_GameEngine::Release()
{
	IGraphics::GetInstance()->Release();
}

void Almond_GameEngine::EngineLoop()
{
	Render();
}

void Almond_GameEngine::Render()
{
	auto graphicsInstance = IGraphics::GetInstance();
	graphicsInstance->BeginRender();
	graphicsInstance->Render();
	graphicsInstance->EndRender();
}
