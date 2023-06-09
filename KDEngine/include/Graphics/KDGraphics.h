#pragma once

#include "WinBase/KDWin.h"
#include "WinBase/KDException.h"
#include "DXErr/DXGIInfoManager.h"
#include "ECS/KDCamera.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx11.h"

#include <vector>
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

namespace KDE
{
	class KDGraphics
	{
		friend class Bindable;
		friend class ImGuiLayer;
		friend class EditorLayer;
		friend class KDWindow;
	public:
		KDGraphics(HWND hWnd, uint32_t width = 800, uint32_t height = 600);
		~KDGraphics() {}

		void EndFrame();
		void ClearBuffer(float red, float green, float blue);
		void DrawIndexed(UINT count);
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
		void OnResize(uint32_t width, uint32_t height);
	public:
		KDCamera Camera;
	private:
		HWND m_WindowHandle;
		DirectX::XMMATRIX m_Projection;
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