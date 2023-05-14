#pragma once

#include "Drawable.h"
#include "Graphics/Bindable/IndexBuffer.h"

namespace KDE
{
	template<typename T>
	class DrawableBase : public Drawable
	{
	protected:
		static bool IsStaticInitialized()
		{
			return !m_StaticBinds.empty();
		}
		static void AddStaticBind(std::unique_ptr<Bindable> bind)
		{
			assert("Must use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer));
			m_StaticBinds.push_back(std::move(bind));
		}
		void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> iBuf)
		{
			assert("Index buffer must be added one time." && pIndexBuffer == nullptr);
			pIndexBuffer = iBuf.get();
			m_StaticBinds.push_back(std::move(iBuf));
		}
		void SetIndexFromStatic()
		{
			assert("Index buffer must be added one time." && pIndexBuffer == nullptr);
			for (const auto& b : m_StaticBinds)
			{
				if (const auto p = dynamic_cast<IndexBuffer*>(b.get()))
				{
					pIndexBuffer = p;
					return;
				}
			}
			assert("Failed to find index buffer in static binds");
		}

	private:
		const std::vector<std::unique_ptr<Bindable>>& StaticBinds() const override
		{
			return m_StaticBinds;
		}
	private:
		static std::vector<std::unique_ptr<Bindable>> m_StaticBinds;
	};

	template<typename T>
	std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::m_StaticBinds;
}