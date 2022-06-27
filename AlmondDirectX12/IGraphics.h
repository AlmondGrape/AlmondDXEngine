#pragma once

#ifdef ALMONDDIRECTX12_EXPORTS
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API __declspec(dllimport)
#endif

using namespace std;

namespace Almond
{
	class IGraphics
	{
	public:
		IGraphics(const IGraphics& other) = delete;
		IGraphics& operator =(const IGraphics& other) = delete;
		IGraphics(IGraphics&& other) = delete;

		static GRAPHICS_API shared_ptr<IGraphics> GetInstance();

		virtual GRAPHICS_API void Initialize(HWND hWnd, int width, int height) abstract;

		virtual GRAPHICS_API void BeginRender() abstract;
		virtual GRAPHICS_API void Render() abstract;
		virtual GRAPHICS_API void EndRender() abstract;

		virtual GRAPHICS_API void Release() abstract;

	protected:
		IGraphics() = default;

	private:
		static shared_ptr<IGraphics> s_pGraphics;

	};
}
