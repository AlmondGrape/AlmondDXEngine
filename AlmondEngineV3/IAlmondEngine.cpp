#include "pch.h"
#include "IAlmondEngine.h"
#include "Almond_GameEngine.h"

using namespace Almond;

shared_ptr<IAlmondEngine> IAlmondEngine::s_pAlmondEngine = nullptr;

shared_ptr<IAlmondEngine> IAlmondEngine::GetInstance()
{
	if (s_pAlmondEngine == nullptr)
	{
		s_pAlmondEngine = std::make_shared<Almond_GameEngine>();
	}

	return s_pAlmondEngine;
}
