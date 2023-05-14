#pragma once

#include "KeyCodes.h"

#include <queue>
#include <bitset>

namespace KDE
{
	class KDKeyboard
	{
		friend class KDWindow;
	public:
		class Event
		{
		public:
			enum class Type
			{
				Invalid = 0,

				Press,
				Release
			};
		public:
			Event();
			Event(Type type, unsigned char code);

			bool IsPress() const;
			bool IsRelease() const;
			bool IsValid() const;
			unsigned char Code() const;

		private:
			Type type;
			unsigned char code;
		};
	public:
		KDKeyboard() = default;
		KDKeyboard(const KDKeyboard&) = delete;
		KDKeyboard& operator = (const KDKeyboard&) = delete;

		bool IsKeyPressed(unsigned char keycode);
		bool IsKeyPressed(Key key);
		Event ReadKey();
		bool IsKeyEmpty() const;
		void ClearKey();

		char ReadChar();
		bool IsCharEmpty();
		void ClearChar();
		void Clear();

		void Autorepeat(bool state);
		bool IsAutorepeatEnabled() const;

	private:
		void OnKeyPressed(unsigned char keycode);
		void OnKeyReleased(unsigned char keycode);
		void OnChar(unsigned char keycode);
		void ClearState();
		template<typename T>
		static void TrimBuffer(std::queue<T>& buffer);
	private:
		static constexpr uint32_t m_Keys = 256;
		static constexpr uint32_t m_BufferSize = 16;
		bool m_Autorepeat = false;
		std::bitset<m_Keys> m_KeyStates;
		std::queue<Event> m_KeyBuffer;
		std::queue<char> m_CharBuffer;
	};
}