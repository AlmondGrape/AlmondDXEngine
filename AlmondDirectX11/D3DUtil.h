#pragma once
#include <d3d11_1.h>
#include <string>
#include "SimpleMath.h"

// 전처리기의 역슬래시 문자는 전처리기에서 줄바꿈을 할 수 있게 해준다. (안함 안됨)
// dxerr.h는 이제 지원하지 않는다. 쓸려면 SDK를 깔아야한다. 그래서 책에서 사용한 처리 말고 다른 걸 사용한다.
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)													\
	{															\
		HRESULT hr = (x);										\
		if(FAILED(hr))											\
		{														\
			LPWSTR output;                                    	\
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |			\
				FORMAT_MESSAGE_IGNORE_INSERTS 	 |				\
				FORMAT_MESSAGE_ALLOCATE_BUFFER,					\
				NULL,											\
				hr,												\
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),		\
				(LPTSTR) &output,								\
				0,												\
				NULL);											\
			MessageBox(NULL, output, L"Error", MB_OK);			\
		}														\
	}
#endif
#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#ifndef ReleaseCOM
#define ReleaseCOM(x) { if(x) {x->Release(); x = 0;}}
#endif

#ifndef SafeDelete
#define SafeDelete(x) { delete x; x = 0;}
#endif

// inline 키워드 상태에 따라 다른 걸로 대체
#ifndef D3DX11INLINE
#ifdef _MSC_VER
#if (_MSC_VER >= 1200)
#define D3DX11INLINE __forceinline
#else
#define D3DX11INLINE __inline
#endif
#else
#ifdef __cplusplus
#define D3DX11INLINE inline
#else
#define D3DX11INLINE
#endif
#endif
#endif

//
// 디버깅 보조 관련 기능
//

// ------------------------------
// D3D11SetDebugObjectName 함수
// ------------------------------
// Direct3D Device에 의해 생성된 개체에 대한 그래픽 디버거에서의 개체 이름을 설정
// [In]resource				D3D11 디바이스에서 생성된 개체
// [In]name					개체 이름
template<UINT TNameLength>
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const char(&name)[TNameLength])
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}

// [In]resource				D3D11 디바이스에서 생성된 개체
// [In]name					개체 이름
// [In]length				문자열 길이
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ LPCSTR name, _In_ UINT length)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, length, name);
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(length);
#endif
}

// [In]resource				D3D11 디바이스에서 생성된 개체
// [In]name					개체 이름
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}

// 그래픽 디버거에서 Direct3D Device에 의해 생성된 개체의 이름을 지우는 함수.
// [In]resource				D3D11 디바이스에서 생성된 개체
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ std::nullptr_t)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
#else
	UNREFERENCED_PARAMETER(resource);
#endif
}

// ------------------------------
// DXGISetDebugObjectName 함수
// ------------------------------
// DXGI 개체에 대한 그래픽 디버거에서의 개체 이름을 설정합니다.
// [In]object				DXGI 개체
// [In]name					개체 이름
template<UINT TNameLength>
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ const char(&name)[TNameLength])
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
#else
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(name);
#endif
}

// [In]object				DXGI 개체
// [In]name					개체 이름
// [In]length				문자열 길이
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ LPCSTR name, _In_ UINT length)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, length, name);
#else
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(name);
	UNREFERENCED_PARAMETER(length);
#endif
}

// [In]object				DXGI 개체
// [In]name					개체 이름
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
#else
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(name);
#endif
}

// 그래픽 디버거에서 DXGI 개체의 이름을 지우는 함수.
// [In]object				DXGI 개체
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ std::nullptr_t)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
#else
	UNREFERENCED_PARAMETER(object);
#endif
}

namespace D3DUtil
{
	D3DX11INLINE UINT32 ConvertDipsToPixels(UINT32 dips, float dpi)
	{
		static const float dipsPerInch = 96.0f;
		return UINT32((float)dips * dpi / dipsPerInch + 0.5f); // Round to nearest integer.
	}

	template<class Interface>
	inline void
		SafeRelease(
			Interface** ppInterfaceToRelease
		)
	{
		if (*ppInterfaceToRelease != NULL)
		{
			(*ppInterfaceToRelease)->Release();

			(*ppInterfaceToRelease) = NULL;
		}
	}

}

