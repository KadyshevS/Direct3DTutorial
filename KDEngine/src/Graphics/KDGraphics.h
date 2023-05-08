#pragma once

#include "WinBase/KDWin.h"
#include "WinBase/KDException.h"
#include "DXErr/DXGIInfoManager.h"

#include <vector>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace KDE
{
	class KDGraphics
	{
		friend class Bindable;
	public:
		KDGraphics(HWND hWnd);
		~KDGraphics() {}

		void EndFrame();
		void ClearBuffer(float red, float green, float blue);
		void DrawIndexed(UINT count);
		void SetProjection(DirectX::FXMMATRIX proj);

		DirectX::XMMATRIX Projection() const;
	public:
		class Exception : public KDException
		{
			using KDException::KDException;
		};
		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {});
			const char* what() const override;
			const char* Type() const override;
			HRESULT ErrorCode() const;
			std::string ErrorString() const;
			std::string ErrorDescription() const;
			std::string ErrorInfo() const;
		private:
			HRESULT hr;
			std::string info;
		};
		class InfoException : public Exception
		{
		public:
			InfoException(int line, const char* file, std::vector<std::string> infoMsgs);
			const char* what() const override;
			const char* Type() const override;
			std::string ErrorInfo() const;
		private:
			std::string info;
		};
		class DeviceRemovedException : public HrException
		{
			using HrException::HrException;
		public:
			const char* Type() const override;
		private:
			std::string reason;
		};

	private:
		DirectX::XMMATRIX m_Projection;
	private:
#ifdef _DEBUG
		DXGIInfoManager m_InfoManager;
#endif
		Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
		Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_Target;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	};
}