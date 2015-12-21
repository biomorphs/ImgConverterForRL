#ifndef IMGCONVERTER_DXT1_TO_BITMAP_CONVERSION_INCLUDED
#define IMGCONVERTER_DXT1_TO_BITMAP_CONVERSION_INCLUDED

#include <memory>

class Image;
class BlockCompressedImage;

class DXT1ToBitmapConversion
{
public:
	DXT1ToBitmapConversion();
	~DXT1ToBitmapConversion();

	std::unique_ptr<Image> Convert(const BlockCompressedImage& sourceImage);
};

#endif