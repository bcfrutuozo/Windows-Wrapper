//#include "Image.h"
//
//Image Image::CreateImageObject(const IntPtr& nativeImage)
//{
//	Image image;
//
//	int type = -1;
//
//	int status = SafeNativeMethods::Gdip->GdipGetImageType(nativeImage);
//
//	if (status != SafeNativeMethods::Gdip->Ok)
//		throw SafeNativeMethods::Gdip->StatusException(status);
//
//	switch ((ImageTypeEnum)type)
//	{
//		case ImageTypeEnum::Bitmap:
//		//image = Bitmap::FromGDIplus(nativeImage);
//		break;
//
//		case ImageTypeEnum::Metafile:
//		//image = Metafile::FromGDIplus(nativeImage);
//		break;
//
//		default:
//		throw ArgumentException("InvalidImage");
//	}
//
//	return image;
//}
