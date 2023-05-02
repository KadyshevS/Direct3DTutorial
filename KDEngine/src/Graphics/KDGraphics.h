#pragma once

#include "WinBase/KDWin.h"
#include "WinBase/KDException.h"

#include <d3d11.h>

namespace KDE
{
	class KDGraphics
	{
	public:
		KDGraphics(HWND hWnd);
		~KDGraphics();

		void EndFrame();
		void ClearBuffer(float red, float green, float blue);
	public:
		class Exception : public KDException
		{
			using KDException::KDException;
		};
		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr);
			const char* what() const override;
			const char* Type() const override;
			HRESULT ErrorCode() const;
			std::string ErrorString() const;
			std::string ErrorDescription() const;
		private:
			HRESULT hr;
		};
		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			const char* Type() const noexcept override;
		};

	private:
		ID3D11Device* m_Device = nullptr;
		IDXGISwapChain* m_SwapChain = nullptr;
		ID3D11DeviceContext* m_Context = nullptr;
		ID3D11RenderTargetView* m_Target = nullptr;
	};
}