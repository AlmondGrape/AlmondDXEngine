#pragma once
#include "IAlmondEngine.h"

using namespace std;

namespace Almond
{
	class Almond_GameEngine : public IAlmondEngine
	{
	public:
		virtual void Initialize(HWND hWnd, int width, int height) override;
		virtual void Release() override;
		virtual void EngineLoop() override;

	private:
		void Render();
			
		

	};
}
