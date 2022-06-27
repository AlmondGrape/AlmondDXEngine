#pragma once
#include <d3d11_1.h>
#include <string>
#include "SimpleMath.h"

// ��ó������ �������� ���ڴ� ��ó���⿡�� �ٹٲ��� �� �� �ְ� ���ش�. (���� �ȵ�)
// dxerr.h�� ���� �������� �ʴ´�. ������ SDK�� ��ƾ��Ѵ�. �׷��� å���� ����� ó�� ���� �ٸ� �� ����Ѵ�.
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

// inline Ű���� ���¿� ���� �ٸ� �ɷ� ��ü
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
// ����� ���� ���� ���
//

// ------------------------------
// D3D11SetDebugObjectName �Լ�
// ------------------------------
// Direct3D Device�� ���� ������ ��ü�� ���� �׷��� ����ſ����� ��ü �̸��� ����
// [In]resource				D3D11 ����̽����� ������ ��ü
// [In]name					��ü �̸�
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

// [In]resource				D3D11 ����̽����� ������ ��ü
// [In]name					��ü �̸�
// [In]length				���ڿ� ����
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

// [In]resource				D3D11 ����̽����� ������ ��ü
// [In]name					��ü �̸�
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
#else
	UNREFERENCED_PARAMETER(resource);
	UNREFERENCED_PARAMETER(name);
#endif
}

// �׷��� ����ſ��� Direct3D Device�� ���� ������ ��ü�� �̸��� ����� �Լ�.
// [In]resource				D3D11 ����̽����� ������ ��ü
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ std::nullptr_t)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, 0, nullptr);
#else
	UNREFERENCED_PARAMETER(resource);
#endif
}

// ------------------------------
// DXGISetDebugObjectName �Լ�
// ------------------------------
// DXGI ��ü�� ���� �׷��� ����ſ����� ��ü �̸��� �����մϴ�.
// [In]object				DXGI ��ü
// [In]name					��ü �̸�
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

// [In]object				DXGI ��ü
// [In]name					��ü �̸�
// [In]length				���ڿ� ����
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

// [In]object				DXGI ��ü
// [In]name					��ü �̸�
inline void DXGISetDebugObjectName(_In_ IDXGIObject* object, _In_ const std::string& name)
{
#if (defined(DEBUG) || defined(_DEBUG)) && (GRAPHICS_DEBUGGER_OBJECT_NAME)
	object->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.length(), name.c_str());
#else
	UNREFERENCED_PARAMETER(object);
	UNREFERENCED_PARAMETER(name);
#endif
}

// �׷��� ����ſ��� DXGI ��ü�� �̸��� ����� �Լ�.
// [In]object				DXGI ��ü
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

