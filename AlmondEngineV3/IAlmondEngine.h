#pragma once
#include "IGraphics.h"

#ifdef ALMONDENGINEV3_EXPORTS
#define ALMOND_ENGINE_API __declspec(dllexport)
#else
#define ALMOND_ENGINE_API __declspec(dllimport)
#endif

// 자주 사용하는 네임스페이스
using namespace std;

namespace Almond
{
	struct WindowInfo;

	// 싱글턴 클래스. 엔진은 하나만 생성.
	// GetInstance로 인스턴스 생성 & 가져오기 가능.
	class IAlmondEngine
	{
	public:
		IAlmondEngine(const IAlmondEngine& other) = delete;
		IAlmondEngine& operator =(const IAlmondEngine& other) = delete;
		IAlmondEngine(IAlmondEngine&& other) = delete;

		static ALMOND_ENGINE_API shared_ptr<IAlmondEngine> GetInstance();

		virtual ALMOND_ENGINE_API void Initialize(HWND hWnd, int width, int height) abstract;
		virtual ALMOND_ENGINE_API void Release() abstract;
		virtual ALMOND_ENGINE_API void EngineLoop() abstract;

	protected:
		IAlmondEngine() = default;
		virtual ~IAlmondEngine() = default;

	private:		
		static shared_ptr<IAlmondEngine> s_pAlmondEngine;

	};
}
