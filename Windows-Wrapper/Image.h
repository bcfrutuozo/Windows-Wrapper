//#pragma once
//
//#include "IDisposable.h"
//#include "IntPtr.h"
//#include "Color.h"
//#include "Exceptions.h"
//#include "Utilities.h"
//
//#include <Shlwapi.h>
//
//class Image : public IDisposable
//{
//protected:
//
//	enum class ImageTypeEnum
//	{
//		Bitmap = 1,
//		Metafile = 2,
//	};
//
//	IntPtr m_NativeImage;
//
//	Image() : m_NativeImage(nullptr) { }
//	Image(IntPtr nativeImage) 
//		: 
//		m_NativeImage(nativeImage) 
//	{
//		Width = nativeImage->GetHeight();
//		Height = nativeImage->GetHeight();
//	}
//
//	int Width;
//	int Height;
//
//	static Image CreateImageObject(const IntPtr& nativeImage);
//
//public:
//
//	void Dispose() override;
//
//	static Image FromFile(const std::string& filename) { return FromFile(filename, false); }
//	static Image FromFile(const std::string& filename, bool useEmbeddedColorManagement)
//	{
//		if (!std::filesystem::exists(filename))
//		{
//			throw FileNotFoundException(filename);
//		}
//
//		std::ifstream file(filename, std::ios::binary);
//
//		BITMAPFILEHEADER bmFileHeader;
//		/*
//			Need to be very careful when read()ing directly
//			into a structure. If the structure has padding
//			bytes, the members will get out of sync with the
//			data in the file. <Windows.h> defines these
//			structs such that there are no padding bytes,
//			so we will have no problems here.
//		*/
//		file.read(reinterpret_cast<char*>(&bmFileHeader),
//				  sizeof(bmFileHeader));
//
//		BITMAPINFOHEADER bmInfoHeader;
//		file.read(reinterpret_cast<char*>(&bmInfoHeader),
//				  sizeof(bmInfoHeader));
//
//		// Assuring that the loaded bitmap has 24 color bits. (RGB)
//		assert(bmInfoHeader.biBitCount == 24);
//		// Assuring that the loaded bitmap has no compression (because it's difficult to deal with). :D
//		assert(bmInfoHeader.biCompression == BI_RGB);
//
//		// Seek the file the desired offset from header.
//		file.seekg(bmFileHeader.bfOffBits);
//
//		// Bitmap padding formula according to its format.
//		const int padding = (4 - (bmInfoHeader.biWidth * 3) % 4) % 4;
//
//		//for (int y = bmInfoHeader.biHeight - 1; y >= 0; y--)
//		//{
//		//	for (int x = 0; x < bmInfoHeader.biWidth; x++)
//		//	{
//		//		BYTE r = file.get();
//		//		BYTE g = file.get();
//		//		BYTE b = file.get();
//		//		COLORREF c = RGB(b, g, r);
//		//		SetPixel(device, x, y + 150, c); // + 150 is the upper offset to avoid chars to override image after loading.
//		//	}
//
//		//	// Add padding at the end of each row.
//		//	file.seekg(padding, std::ios::cur);
//		//}
//	}
//};
//
