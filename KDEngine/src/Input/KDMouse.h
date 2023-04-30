#pragma once

#include <utility>
#include <queue>

namespace KDE
{
	class KDMouse
	{
		friend class KDWindow;
	public:
		class Event
		{
		public:
			enum class EventType
			{
				Invalid = 0,

				LeftPress,
				LeftRelease,
				RightPress,
				RightRelease,
				WheelDown,
				WheelUp,
				Enter,
				Leave,
				Move
			};
		public:
			Event();
			Event(EventType type, const KDMouse& parent);

			bool IsValid() const;
			EventType Type() const;
			std::pair<int, int> Position() const;
			int PositionX() const;
			int PositionY() const;
			bool IsLeftIsPressed() const;
			bool IsRightIsPressed() const;

		private:
			EventType type;
			bool LeftIsPressed;
			bool RightIsPressed;
			int x, y;
		};
	public:
		KDMouse() = default;
		KDMouse(const KDMouse&) = delete;
		KDMouse& operator = (const KDMouse&) = delete;

		std::pair<int, int> Position() const;
		int PositionX() const;
		int PositionY() const;
		bool IsInWindow() const;
		bool IsLeftIsPressed() const;
		bool IsRightIsPressed() const;
		Event Read();
		bool IsEmpty();
		void Clear();
		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);

	private:
		void OnLeftPressed();
		void OnRightPressed();
		void OnLeave();
		void OnEnter();
		void OnLeftReleased();
		void OnRightReleased();
		void OnWheelDown(int x, int y);
		void OnWheelUp(int x, int y);
		void OnMove(int x, int y);
	private:
		static constexpr uint32_t m_BufferSize = 16;
		int m_PositionX = 0, m_PositionY = 0;
		bool m_LeftIsPressed = false, m_RightIsPressed = false;
		bool m_IsInWindow = false;
		std::queue<Event> m_Buffer;
	};
}