#include "KDMouse.h"

namespace KDE
{
//////////////////////////////////////////////////////////////////////////
////	Mouse
	std::pair<int, int> KDMouse::Position() const
	{
		return std::pair(m_PositionX, m_PositionY);
	}
	int KDMouse::PositionX() const
	{
		return m_PositionX;
	}
	int KDMouse::PositionY() const
	{
		return m_PositionY;
	}
	bool KDMouse::IsInWindow() const
	{
		return m_IsInWindow;
	}
	bool KDMouse::IsLeftIsPressed() const
	{
		return m_LeftIsPressed;
	}
	bool KDMouse::IsRightIsPressed() const
	{
		return m_RightIsPressed;
	}

	KDMouse::Event KDMouse::Read()
	{
		if (m_Buffer.size() > 0)
		{
			auto e = m_Buffer.front();
			m_Buffer.pop();
			return e;
		}
		else
			return KDMouse::Event();
	}
	bool KDMouse::IsEmpty()
	{
		return m_Buffer.empty();
	}
	void KDMouse::Clear()
	{
		m_Buffer = std::queue<KDMouse::Event>();
	}

	void KDMouse::OnLeftPressed()
	{
		m_LeftIsPressed = true;

		m_Buffer.push({ KDMouse::Event::EventType::LeftPress, *this });
		TrimBuffer(m_Buffer);
	}
	void KDMouse::OnRightPressed()
	{
		m_RightIsPressed = true;

		m_Buffer.push({ KDMouse::Event::EventType::RightPress, *this});
		TrimBuffer(m_Buffer);
	}

	void KDMouse::OnLeave()
	{
		m_IsInWindow = false;
		m_Buffer.push({ KDMouse::Event::EventType::Leave, *this });
		TrimBuffer(m_Buffer);
	}
	void KDMouse::OnEnter()
	{
		m_IsInWindow = true;
		m_Buffer.push({ KDMouse::Event::EventType::Enter, *this });
		TrimBuffer(m_Buffer);
	}

	void KDMouse::OnLeftReleased()
	{
		m_LeftIsPressed = false;

		m_Buffer.push({ KDMouse::Event::EventType::LeftRelease, *this });
		TrimBuffer(m_Buffer);
	}
	void KDMouse::OnRightReleased()
	{
		m_RightIsPressed = false;

		m_Buffer.push({ KDMouse::Event::EventType::RightRelease, *this });
		TrimBuffer(m_Buffer);
	}
	void KDMouse::OnWheelDown(int x, int y)
	{
		m_Buffer.push({ KDMouse::Event::EventType::WheelDown, *this });
		TrimBuffer(m_Buffer);
	}
	void KDMouse::OnWheelUp(int x, int y)
	{
		m_Buffer.push({ KDMouse::Event::EventType::WheelUp, *this });
		TrimBuffer(m_Buffer);
	}
	void KDMouse::OnMove(int x, int y)
	{
		m_PositionX = x;
		m_PositionY = y;

		m_Buffer.push({ KDMouse::Event::EventType::Move, *this });
		TrimBuffer(m_Buffer);
	}
	template<typename T>
	void KDMouse::TrimBuffer(std::queue<T>& buffer)
	{
		while (buffer.size() > m_BufferSize)
			buffer.pop();
	}
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
////	Mouse Event
	KDMouse::Event::Event()
		: type(EventType::Invalid),
		LeftIsPressed(false),
		RightIsPressed(false),
		x(0), y(0)
	{}
	KDMouse::Event::Event(EventType type, const KDMouse& parent)
		: type(type),
		LeftIsPressed(parent.m_LeftIsPressed),
		RightIsPressed(parent.m_RightIsPressed),
		x(parent.m_PositionX), y(parent.m_PositionY)
	{}

	bool KDMouse::Event::IsValid() const
	{
		return type != EventType::Invalid;
	}
	KDMouse::Event::EventType KDMouse::Event::Type() const
	{
		return type;
	}

	std::pair<int, int> KDMouse::Event::Position() const
	{
		return std::pair(x, y);
	}
	int KDMouse::Event::PositionX() const
	{
		return x;
	}
	int KDMouse::Event::PositionY() const
	{
		return y;
	}

	bool KDMouse::Event::IsLeftIsPressed() const
	{
		return LeftIsPressed;
	}
	bool KDMouse::Event::IsRightIsPressed() const
	{
		return RightIsPressed;
	}
//////////////////////////////////////////////////////////////////////////
}