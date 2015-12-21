#include "dxt1_to_bitmap_conversion.h"
#include "image.h"
#include "block_compressed_image.h"

DXT1ToBitmapConversion::DXT1ToBitmapConversion()
{

}

DXT1ToBitmapConversion::~DXT1ToBitmapConversion()
{

}

std::unique_ptr<Image> DXT1ToBitmapConversion::Convert(const BlockCompressedImage& sourceImage)
{
	if (sourceImage.GetWidthBlocks() == 0 || sourceImage.GetHeightBlocks() == 0)
	{
		return nullptr;
	}

	// Make a new image object to contain the converted data
	auto newImage = std::make_unique<Image>(sourceImage.GetWidthPixels(), sourceImage.GetHeightPixels());
	Assert(newImage != nullptr);

	// Convert one block at a time
	const uint32_t c_imgWidthBlocks = sourceImage.GetWidthBlocks();
	const uint32_t c_imgHeightBlocks = sourceImage.GetHeightBlocks();
	for (uint32_t y = 0; y < c_imgHeightBlocks; ++y)
	{
		for (uint32_t x = 0; x < c_imgWidthBlocks; ++x)
		{
			const auto thisBlock = sourceImage.BlockAt(x, y);
			BlockCompressedPixels::ColourLUT lut(*thisBlock);
			for (uint32_t py = 0; py < 4; ++py)
			{
				for (uint32_t px = 0; px < 4; ++px)
				{
					auto pixelColour = lut.GetColour(thisBlock->GetPixelColourIndex(px, py));
					newImage->SetPixelColour((x * 4) + px, (y * 4) + py, pixelColour);
				}
			}
		}
	}

	return newImage;
}