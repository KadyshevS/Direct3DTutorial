#pragma once

#include "WinBase/KDWin.h"
#include "WinBase/KDException.h"

#include <string>
#include <assert.h>
#include <memory>

namespace KDE
{
	class KDSurface
	{
	public:
		class Color
		{
		public:
			unsigned int dword;
		public:
			constexpr Color()  : dword()
			{}
			constexpr Color(const Color& col) 
				:
				dword(col.dword)
			{}
			constexpr Color(unsigned int dw) 
				:
				dword(dw)
			{}
			constexpr Color(unsigned char x, unsigned char r, unsigned char g, unsigned char b) 
				:
				dword((x << 24u) | (r << 16u) | (g << 8u) | b)
			{}
			constexpr Color(unsigned char r, unsigned char g, unsigned char b) 
				:
				dword((r << 16u) | (g << 8u) | b)
			{}
			constexpr Color(Color col, unsigned char x) 
				:
				Color((x << 24u) | col.dword)
			{}
			Color& operator =(Color color) 
			{
				dword = color.dword;
				return *this;
			}
			constexpr unsigned char GetX() const 
			{
				return dword >> 24u;
			}
			constexpr unsigned char GetA() const 
			{
				return GetX();
			}
			constexpr unsigned char GetR() const 
			{
				return (dword >> 16u) & 0xFFu;
			}
			constexpr unsigned char GetG() const 
			{
				return (dword >> 8u) & 0xFFu;
			}
			constexpr unsigned char GetB() const 
			{
				return dword & 0xFFu;
			}
			void SetX(unsigned char x) 
			{
				dword = (dword & 0xFFFFFFu) | (x << 24u);
			}
			void SetA(unsigned char a) 
			{
				SetX(a);
			}
			void SetR(unsigned char r) 
			{
				dword = (dword & 0xFF00FFFFu) | (r << 16u);
			}
			void SetG(unsigned char g) 
			{
				dword = (dword & 0xFFFF00FFu) | (g << 8u);
			}
			void SetB(unsigned char b) 
			{
				dword = (dword & 0xFFFFFF00u) | b;
			}
		};
	public:
		class Exception : public KDException
		{
		public:
			Exception(int line, const char* file, std::string note);
			const char* what() const override;
			const char* Type() const override;
			const std::string& GetNote() const;
		private:
			std::string note;
		};
	public:
		KDSurface(unsigned int width, unsigned int height, unsigned int pitch);
		KDSurface(unsigned int width, unsigned int height);
		KDSurface(KDSurface&& source);
		KDSurface(KDSurface&) = delete;
		KDSurface& operator=(KDSurface&& donor);
		KDSurface& operator=(const KDSurface&) = delete;
		~KDSurface();
		void Clear(Color fillValue);
		void PutPixel(unsigned int x, unsigned int y, Color c);
		Color GetPixel(unsigned int x, unsigned int y) const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		Color* GetBufferPtr();
		const Color* GetBufferPtr() const;
		const Color* GetBufferPtrConst() const;
		static KDSurface FromFile(const std::string& name);
		void Save(const std::string& filename) const;
		void Copy(const KDSurface& src);
	private:
		KDSurface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam) ;
	private:
		std::unique_ptr<Color[]> pBuffer;
		unsigned int width;
		unsigned int height;
	};
}
