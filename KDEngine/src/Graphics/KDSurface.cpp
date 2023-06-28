#define KD_INCLUDE_WINAPI_FULL
#include "Graphics/KDSurface.h"
#include <algorithm>
namespace Gdiplus
{
	using std::min;
	using std::max;
}
#include <gdiplus.h>
#include <sstream>

#pragma comment( lib,"gdiplus.lib" )

namespace KDE
{
	KDSurface::KDSurface(unsigned int width, unsigned int height)
		:
		pBuffer(std::make_unique<Color[]>(width* height)),
		width(width),
		height(height)
	{}

	KDSurface& KDSurface::operator=(KDSurface&& donor)
	{
		width = donor.width;
		height = donor.height;
		pBuffer = std::move(donor.pBuffer);
		donor.pBuffer = nullptr;
		return *this;
	}

	KDSurface::KDSurface(KDSurface&& source)
		:
		pBuffer(std::move(source.pBuffer)),
		width(source.width),
		height(source.height)
	{}

	KDSurface::~KDSurface()
	{}

	void KDSurface::Clear(Color fillValue)
	{
		memset(pBuffer.get(), fillValue.dword, width * height * sizeof(Color));
	}

	void KDSurface::PutPixel(unsigned int x, unsigned int y, Color c)
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(x < width);
		assert(y < height);
		pBuffer[y * width + x] = c;
	}

	KDSurface::Color KDSurface::GetPixel(unsigned int x, unsigned int y) const
	{
		assert(x >= 0);
		assert(y >= 0);
		assert(x < width);
		assert(y < height);
		return pBuffer[y * width + x];
	}

	unsigned int KDSurface::GetWidth() const
	{
		return width;
	}

	unsigned int KDSurface::GetHeight() const
	{
		return height;
	}

	KDSurface::Color* KDSurface::GetBufferPtr()
	{
		return pBuffer.get();
	}

	const KDSurface::Color* KDSurface::GetBufferPtr() const
	{
		return pBuffer.get();
	}

	const KDSurface::Color* KDSurface::GetBufferPtrConst() const
	{
		return pBuffer.get();
	}

	KDSurface KDSurface::FromFile(const std::string& name)
	{
		unsigned int width = 0;
		unsigned int height = 0;
		std::unique_ptr<Color[]> pBuffer;

		{
			// convert filenam to wide string (for Gdiplus)
			wchar_t wideName[512];
			mbstowcs_s(nullptr, wideName, name.c_str(), _TRUNCATE);

			Gdiplus::Bitmap bitmap(wideName);
			if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
			{
				std::stringstream ss;
				ss << "Loading image [" << name << "]: failed to load.";
				throw Exception(__LINE__, __FILE__, ss.str());
			}

			width = bitmap.GetWidth();
			height = bitmap.GetHeight();
			pBuffer = std::make_unique<Color[]>(width * height);

			for (unsigned int y = 0; y < height; y++)
			{
				for (unsigned int x = 0; x < width; x++)
				{
					Gdiplus::Color c;
					bitmap.GetPixel(x, y, &c);
					pBuffer[y * width + x] = c.GetValue();
				}
			}
		}

		return KDSurface(width, height, std::move(pBuffer));
	}

	void KDSurface::Save(const std::string& filename) const
	{
		auto GetEncoderClsid = [&filename](const WCHAR* format, CLSID* pClsid) -> void
		{
			UINT  num = 0;          // number of image encoders
			UINT  size = 0;         // size of the image encoder array in bytes

			Gdiplus::ImageCodecInfo* pImageCodecInfo = nullptr;

			Gdiplus::GetImageEncodersSize(&num, &size);
			if (size == 0)
			{
				std::stringstream ss;
				ss << "Saving surface to [" << filename << "]: failed to get encoder; size == 0.";
				throw Exception(__LINE__, __FILE__, ss.str());
			}

			pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
			if (pImageCodecInfo == nullptr)
			{
				std::stringstream ss;
				ss << "Saving surface to [" << filename << "]: failed to get encoder; failed to allocate memory.";
				throw Exception(__LINE__, __FILE__, ss.str());
			}

			GetImageEncoders(num, size, pImageCodecInfo);

			for (UINT j = 0; j < num; ++j)
			{
				if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
				{
					*pClsid = pImageCodecInfo[j].Clsid;
					free(pImageCodecInfo);
					return;
				}
			}

			free(pImageCodecInfo);
			std::stringstream ss;
			ss << "Saving surface to [" << filename <<
				"]: failed to get encoder; failed to find matching encoder.";
			throw Exception(__LINE__, __FILE__, ss.str());
		};

		CLSID bmpID;
		GetEncoderClsid(L"image/bmp", &bmpID);

		wchar_t wideName[512];
		mbstowcs_s(nullptr, wideName, filename.c_str(), _TRUNCATE);

		Gdiplus::Bitmap bitmap(width, height, width * sizeof(Color), PixelFormat32bppARGB, (BYTE*)pBuffer.get());
		if (bitmap.Save(wideName, &bmpID, nullptr) != Gdiplus::Status::Ok)
		{
			std::stringstream ss;
			ss << "Saving surface to [" << filename << "]: failed to save.";
			throw Exception(__LINE__, __FILE__, ss.str());
		}
	}

	void KDSurface::Copy(const KDSurface& src)
	{
		assert(width == src.width);
		assert(height == src.height);
		memcpy(pBuffer.get(), src.pBuffer.get(), width * height * sizeof(Color));
	}

	KDSurface::KDSurface(unsigned int width, unsigned int height, std::unique_ptr<Color[]> pBufferParam)
		:
		width(width),
		height(height),
		pBuffer(std::move(pBufferParam))
	{}


	// surface exception stuff
	KDSurface::Exception::Exception(int line, const char* file, std::string note)
		:
		KDException(line, file),
		note(std::move(note))
	{}

	const char* KDSurface::Exception::what() const
	{
		std::ostringstream oss;
		oss << KDException::what() << std::endl
			<< "[Note] " << GetNote();
		m_WhatBuffer = oss.str();
		return m_WhatBuffer.c_str();
	}

	const char* KDSurface::Exception::Type() const
	{
		return "Chili Graphics Exception";
	}

	const std::string& KDSurface::Exception::GetNote() const
	{
		return note;
	}
}
