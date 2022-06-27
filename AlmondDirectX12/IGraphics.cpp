#include "pch.h"
#include "IGraphics.h"
#include "Graphics_DX12.h"

using namespace Almond;

std::shared_ptr<IGraphics> IGraphics::s_pGraphics = nullptr;

GRAPHICS_API shared_ptr<IGraphics> IGraphics::GetInstance()
{
	if (s_pGraphics == nullptr)
		s_pGraphics = std::make_shared<Graphics_DX12>();

	return s_pGraphics;

}
