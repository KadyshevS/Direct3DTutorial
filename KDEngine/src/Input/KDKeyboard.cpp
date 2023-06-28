#include "Input/KDKeyboard.h"

namespace KDE
{
	KDKeyboard::Event::Event(Type type, unsigned char code)
		: type(type), code(code) {}
	KDKeyboard::Event::Event()
		: type(Type::Invalid), code(0) {}

	bool KDKeyboard::Event::IsPress() const
	{
		return type == Type::Press;
	}
	bool KDKeyboard::Event::IsRelease() const
	{
		return type == Type::Release;
	}
	bool KDKeyboard::Event::IsValid() const
	{
		return type != Type::Invalid;
	}
	unsigned char KDKeyboard::Event::Code() const
	{
		return code;
	}

	bool KDKeyboard::IsKeyPressed(unsigned char keycode)
	{
		return m_KeyStates[keycode];
	}
	bool KDKeyboard::IsKeyPressed(Key key)
	{
		return IsKeyPressed((char)key);
	}
	KDKeyboard::Event KDKeyboard::ReadKey()
	{
		if (m_KeyBuffer.size() > 0)
		{
			KDKeyboard::Event e = m_KeyBuffer.front();
			m_KeyBuffer.pop();
			return e;
		}
		else
			return KDKeyboard::Event();
	}
	bool KDKeyboard::IsKeyEmpty() const
	{
		return m_KeyBuffer.empty();
	}

	char KDKeyboard::ReadChar()
	{
		if (m_CharBuffer.size() > 0)
		{
			unsigned char key = m_CharBuffer.front();
			m_CharBuffer.pop();
			return key;
		}
		else
			return 9;
	}
	bool KDKeyboard::IsCharEmpty()
	{
		return m_CharBuffer.empty();
	}

	void KDKeyboard::ClearKey()
	{
		m_KeyBuffer = std::queue<Event>();
	}
	void KDKeyboard::ClearChar()
	{
		m_CharBuffer = std::queue<char>();
	}
	void KDKeyboard::Clear()
	{
		ClearKey();
		ClearChar();
	}

	void KDKeyboard::Autorepeat(bool state)
	{
		m_Autorepeat = state;
	}
	bool KDKeyboard::IsAutorepeatEnabled() const
	{
		return m_Autorepeat;
	}

	void KDKeyboard::OnKeyPressed(unsigned char keycode)
	{
		m_KeyStates[keycode] = true;
		m_KeyBuffer.push({ KDKeyboard::Event::Type::Press, keycode });
		TrimBuffer(m_KeyBuffer);
	}
	void KDKeyboard::OnKeyReleased(unsigned char keycode)
	{
		m_KeyStates[keycode] = false;
		m_KeyBuffer.push({ KDKeyboard::Event::Type::Release, keycode });
		TrimBuffer(m_KeyBuffer);
	}
	void KDKeyboard::OnChar(unsigned char keycode)
	{
		m_CharBuffer.push(keycode);
		TrimBuffer(m_CharBuffer);
	}
	void KDKeyboard::ClearState()
	{
		m_KeyStates.reset();
	}
	template<typename T>
	void KDKeyboard::TrimBuffer(std::queue<T>& buffer)
	{
		while (buffer.size() > m_BufferSize)
			buffer.pop();
	}
}