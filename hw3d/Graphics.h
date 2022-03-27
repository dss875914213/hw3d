#pragma once

#include "ChiliWin.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;
private:
	ID3D11Device*				m_pDevice = nullptr;
	IDXGISwapChain*				m_pSwap = nullptr;
	ID3D11DeviceContext*		m_pContext = nullptr;
	ID3D11RenderTargetView*		m_pTarget = nullptr;
};

