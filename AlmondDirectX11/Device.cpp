#include "pch.h"
#include "Device.h"
#include "Graphics_DX11.h"

void Almond::Device::Initialize(WindowInfo& info)
{

	HRESULT _hr = S_FALSE;
	UINT _createDeviceFlag = 0;

	// 디버그용 플래그
#if defined(DEBUG) || defined(_DEBUG)
	_createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Direct3D 드라이버 유형의 배열
	D3D_DRIVER_TYPE _driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT _numDriverTypes = ARRAYSIZE(_driverTypes);

	// Direct3D Feature level 유형의 배열
	D3D_FEATURE_LEVEL _featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT _numFeatureLevels = ARRAYSIZE(_featureLevels);

	D3D_DRIVER_TYPE _d3dDriverType;
	D3D_FEATURE_LEVEL _featureLevel;

	// 1. Direct3D의 디바이스와 디바이스 컨텍스트를 생성한다.
	for (UINT driverTypeIndex = 0; driverTypeIndex < _numDriverTypes; driverTypeIndex++)
	{
		_d3dDriverType = _driverTypes[driverTypeIndex];

		_hr = D3D11CreateDevice(nullptr, _d3dDriverType, nullptr, _createDeviceFlag, _featureLevels, _numFeatureLevels,
			D3D11_SDK_VERSION, m_pD3DDevice.GetAddressOf(), &_featureLevel, m_pD3DDeviceContext.GetAddressOf());

		if (_hr == E_INVALIDARG)
		{
			// Direct3D 11.0 API는 D3D_FEATURE_LEVEL_11_1을 인식하지 못하므로 기능 수준 11.0 이하를 시도해야 합니다.
			_hr = D3D11CreateDevice(nullptr, _d3dDriverType, nullptr, _createDeviceFlag, &_featureLevels[1], _numFeatureLevels - 1,
				D3D11_SDK_VERSION, m_pD3DDevice.GetAddressOf(), &_featureLevel, m_pD3DDeviceContext.GetAddressOf());
		}

		if (SUCCEEDED(_hr))
			break;
	}

	// 생성에 실패하였다면 메시지 박스 출현
	if (FAILED(_hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return;
	}

	// Direct3D 11의 Feature Level이 지원되지 않는다면 메시지 박스 출현
	if ((_featureLevel != D3D_FEATURE_LEVEL_11_0) && (_featureLevel != D3D_FEATURE_LEVEL_11_1))
	{
		MessageBox(0, L"Direct3D Feature Level 11 Unsupported.", 0, 0);
		return;
	}

	// 2. 4X MSAA 품질 수준을 지원하는지 점검한다.
	HR(m_pD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &info.MSAAQuality));

	// 4X MSAA가 항상 지원되므로 반환된 품질 수준의 값은 반드시 0보다 커야 한다. 따라서, 0보다 작거나 같으면 assert !
	assert(info.MSAAQuality > 0);

	Microsoft::WRL::ComPtr<IDXGIDevice> _dxgiDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> _dxgiAdapter = nullptr;	

	// Adapter 및 Factory 생성.	
	HR(m_pD3DDevice.As(&_dxgiDevice));
	HR(_dxgiDevice->GetAdapter(_dxgiAdapter.GetAddressOf()));	

	HR(_dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(m_pDxgi.GetAddressOf())));

	// Direct3D 응용 프로그램의 기본 설정 값인 Alt + Enter 전체 화면으로의 전환을 비활성화합니다.
	m_pDxgi->MakeWindowAssociation(info.hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

	_dxgiDevice.ReleaseAndGetAddressOf();

}

void Almond::Device::Release()
{
	
	m_pDxgi.ReleaseAndGetAddressOf();
	m_pD3DDevice.ReleaseAndGetAddressOf();
	m_pD3DDeviceContext.ReleaseAndGetAddressOf();
	
}
