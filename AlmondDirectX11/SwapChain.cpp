#include "pch.h"
#include "Graphics_DX11.h"
#include "SwapChain.h"
#include "Device.h"
#include "RenderTarget.h"

using namespace Almond;

void SwapChain::Initialize(std::shared_ptr<Device> device, WindowInfo& info)
{
	m_pBackBuffer = std::make_shared<RenderTarget>();

	ComPtr<IDXGIFactory2> _dxgiFactory2 = nullptr;
	HRESULT hr = device->GetDXGI().As(&_dxgiFactory2);

	// Direct3D '11.1' 을 적용한다.
	if (_dxgiFactory2 != nullptr)
	{
		// 스왑체인 Descriptor 구조체를 목적에 맞게 서술하고 스왑 체인을 생성한다.
		DXGI_SWAP_CHAIN_DESC1 _sd;
		ZeroMemory(&_sd, sizeof(_sd));
		_sd.BufferCount = 1;
		_sd.Width = info.width;
		_sd.Height = info.height;
		_sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		// 4X MSAA 적용 여부에 따른 서술
		if (info.enable4xMSAA)
		{
			_sd.SampleDesc.Count = 4;
			_sd.SampleDesc.Quality = info.MSAAQuality - 1;
		}
		else
		{
			_sd.SampleDesc.Count = 1;
			_sd.SampleDesc.Quality = 0;
		}

		_sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		_sd.Flags = 0;

		DXGI_SWAP_CHAIN_FULLSCREEN_DESC _fd;
		_fd.RefreshRate.Numerator = 60;
		_fd.RefreshRate.Denominator = 1;
		_fd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		_fd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		_fd.Windowed = TRUE;

		// 서술한 스왑 체인 Descriptor를 적용하여 윈도우 창에 대한 스왑 체인을 생성한다.
		HR(_dxgiFactory2->CreateSwapChainForHwnd(device->GetDevice().Get(), info.hWnd, &_sd, &_fd, nullptr, m_pSwapChain1.GetAddressOf()));
		HR(m_pSwapChain1.As(&m_pSwapChain));
	}
	// Direct3D '11.0' 을 적용한다.
	else
	{
		// 스왑체인 Descriptor 구조체를 목적에 맞게 서술하고 스왑 체인을 생성한다.
		DXGI_SWAP_CHAIN_DESC _sd;
		ZeroMemory(&_sd, sizeof(_sd));
		_sd.BufferCount = 1;
		_sd.BufferDesc.Width = info.width;;
		_sd.BufferDesc.Height = info.height;
		_sd.BufferDesc.RefreshRate.Numerator = 60;
		_sd.BufferDesc.RefreshRate.Denominator = 1;
		_sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		_sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		// 4X MSAA 적용 여부에 따른 서술
		if (info.enable4xMSAA)
		{
			_sd.SampleDesc.Count = 4;
			_sd.SampleDesc.Quality = info.MSAAQuality - 1;
		}
		else
		{
			_sd.SampleDesc.Count = 1;
			_sd.SampleDesc.Quality = 0;
		}

		_sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_sd.OutputWindow = info.hWnd;
		_sd.Windowed = TRUE;
		_sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		_sd.Flags = 0;

		HR(device->GetDXGI()->CreateSwapChain(device->GetDevice().Get(), &_sd, m_pSwapChain.GetAddressOf()));
	}

	_dxgiFactory2.ReleaseAndGetAddressOf(); // 필요없어진 dxgi팩토리 삭제

	// 윈도우 크기에 영향 받는 리소스(RTV 등) 초기화 & 생성
	SetWindowDependentResource(device, info.width, info.height );
}

void SwapChain::BeginRenderToBackBuffer(ComPtr<ID3D11DeviceContext> context)
{
	assert(context);
	assert(m_pSwapChain);

	m_pBackBuffer->ClearRenderTarget(context);
	m_pBackBuffer->SetRenderTarget(context);
}

void SwapChain::Present()
{
	HR(m_pSwapChain->Present(0, 0));
}

void SwapChain::SetWindowDependentResource(std::shared_ptr<Device> device, int screenWidth, int screenHeight)
{
	ComPtr<ID3D11Device> _device = device->GetDevice();
	ComPtr<ID3D11DeviceContext> _context = device->GetDeviceContext();

	// 3개중 하나라도 없으면 뭔가 잘못된 것. 없으면 오류를 강제한다.
	assert(_device);
	assert(_context);
	assert(m_pSwapChain);

	// 렌더링 파이프라인 출력에서 사용하는 관련 리소스(렌더타겟에 있음)를 해제 & 초기화
	m_pBackBuffer->Release();
	m_pBackBuffer->Initialize(_device, screenWidth, screenHeight, true); // 테스트용 일단 댑스버퍼만.

	// 윈도우 사이즈에 맞춰 렌더타겟뷰 생성 & 렌더타겟 인스턴스에 셋팅.
	ComPtr<ID3D11Texture2D> _backBuffer;
	ComPtr<ID3D11RenderTargetView> _rtv;

	HR(m_pSwapChain->ResizeBuffers(1, screenWidth, screenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(_backBuffer.GetAddressOf())));

	_device->CreateRenderTargetView(_backBuffer.Get(), nullptr, _rtv.GetAddressOf());
	m_pBackBuffer->SetRTV(_rtv);
}

void SwapChain::Release()
{
	m_pSwapChain.ReleaseAndGetAddressOf();
	m_pSwapChain1.ReleaseAndGetAddressOf();

	m_pBackBuffer->Release();
}