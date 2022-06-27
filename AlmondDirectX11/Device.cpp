#include "pch.h"
#include "Device.h"
#include "Graphics_DX11.h"

void Almond::Device::Initialize(WindowInfo& info)
{

	HRESULT _hr = S_FALSE;
	UINT _createDeviceFlag = 0;

	// ����׿� �÷���
#if defined(DEBUG) || defined(_DEBUG)
	_createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Direct3D ����̹� ������ �迭
	D3D_DRIVER_TYPE _driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT _numDriverTypes = ARRAYSIZE(_driverTypes);

	// Direct3D Feature level ������ �迭
	D3D_FEATURE_LEVEL _featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT _numFeatureLevels = ARRAYSIZE(_featureLevels);

	D3D_DRIVER_TYPE _d3dDriverType;
	D3D_FEATURE_LEVEL _featureLevel;

	// 1. Direct3D�� ����̽��� ����̽� ���ؽ�Ʈ�� �����Ѵ�.
	for (UINT driverTypeIndex = 0; driverTypeIndex < _numDriverTypes; driverTypeIndex++)
	{
		_d3dDriverType = _driverTypes[driverTypeIndex];

		_hr = D3D11CreateDevice(nullptr, _d3dDriverType, nullptr, _createDeviceFlag, _featureLevels, _numFeatureLevels,
			D3D11_SDK_VERSION, m_pD3DDevice.GetAddressOf(), &_featureLevel, m_pD3DDeviceContext.GetAddressOf());

		if (_hr == E_INVALIDARG)
		{
			// Direct3D 11.0 API�� D3D_FEATURE_LEVEL_11_1�� �ν����� ���ϹǷ� ��� ���� 11.0 ���ϸ� �õ��ؾ� �մϴ�.
			_hr = D3D11CreateDevice(nullptr, _d3dDriverType, nullptr, _createDeviceFlag, &_featureLevels[1], _numFeatureLevels - 1,
				D3D11_SDK_VERSION, m_pD3DDevice.GetAddressOf(), &_featureLevel, m_pD3DDeviceContext.GetAddressOf());
		}

		if (SUCCEEDED(_hr))
			break;
	}

	// ������ �����Ͽ��ٸ� �޽��� �ڽ� ����
	if (FAILED(_hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return;
	}

	// Direct3D 11�� Feature Level�� �������� �ʴ´ٸ� �޽��� �ڽ� ����
	if ((_featureLevel != D3D_FEATURE_LEVEL_11_0) && (_featureLevel != D3D_FEATURE_LEVEL_11_1))
	{
		MessageBox(0, L"Direct3D Feature Level 11 Unsupported.", 0, 0);
		return;
	}

	// 2. 4X MSAA ǰ�� ������ �����ϴ��� �����Ѵ�.
	HR(m_pD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &info.MSAAQuality));

	// 4X MSAA�� �׻� �����ǹǷ� ��ȯ�� ǰ�� ������ ���� �ݵ�� 0���� Ŀ�� �Ѵ�. ����, 0���� �۰ų� ������ assert !
	assert(info.MSAAQuality > 0);

	Microsoft::WRL::ComPtr<IDXGIDevice> _dxgiDevice = nullptr;
	Microsoft::WRL::ComPtr<IDXGIAdapter> _dxgiAdapter = nullptr;	

	// Adapter �� Factory ����.	
	HR(m_pD3DDevice.As(&_dxgiDevice));
	HR(_dxgiDevice->GetAdapter(_dxgiAdapter.GetAddressOf()));	

	HR(_dxgiAdapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(m_pDxgi.GetAddressOf())));

	// Direct3D ���� ���α׷��� �⺻ ���� ���� Alt + Enter ��ü ȭ�������� ��ȯ�� ��Ȱ��ȭ�մϴ�.
	m_pDxgi->MakeWindowAssociation(info.hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES);

	_dxgiDevice.ReleaseAndGetAddressOf();

}

void Almond::Device::Release()
{
	
	m_pDxgi.ReleaseAndGetAddressOf();
	m_pD3DDevice.ReleaseAndGetAddressOf();
	m_pD3DDeviceContext.ReleaseAndGetAddressOf();
	
}
