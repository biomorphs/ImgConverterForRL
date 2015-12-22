#include "catch/catch.hpp"
#include "../ImgLib/dxt1_to_bitmap_conversion.h"
#include "../ImgLib/image.h"
#include "../ImgLib/block_compressed_image.h"

// Test helper
void FillBlockWithColour(BlockCompressedImage& image, uint32_t blckX, uint32_t blckY, const ColourRGB& colour)
{
	auto theBlock = image.BlockAt(blckX, blckY);
	theBlock->SetRefColour1(colour);
	theBlock->SetRefColour2(colour);
	for (uint32_t y = 0; y < 4; ++y)
	{
		for (uint32_t x = 0; x < 4; ++x)
		{
			theBlock->SetPixelColourIndex(x, y, 0);
		}
	}
}

// Test helper
bool BlockContainsValue(BlockCompressedImage& image, uint32_t blckX, uint32_t blckY, const ColourRGB& colour)
{
	auto theBlock = image.BlockAt(blckX, blckY);
	BlockCompressedPixels::ColourLUT lut(*theBlock);

	for (uint32_t y = 0; y < 4; ++y)
	{
		for (uint32_t x = 0; x < 4; ++x)
		{
			auto colourIndex = theBlock->GetPixelColourIndex(x, y);
			if (lut.GetColour(colourIndex) != colour)
			{
				return false;
			}
		}
	}
	return true;
}

TEST_CASE("Test simple conversion")
{
	ColourRGB c_refColour(0, 255, 0);
	BlockCompressedImage testImage(4,4);
	FillBlockWithColour(testImage, 0, 0, c_refColour);

	DXT1ToBitmapConverter converter;
	auto convertedImage = converter.Convert(testImage);
	REQUIRE(convertedImage.get() != nullptr);
	REQUIRE(convertedImage->GetWidthPixels() == 4);
	REQUIRE(convertedImage->GetHeightPixels() == 4);

	REQUIRE(BlockContainsValue(testImage, 0, 0, c_refColour) == true);
}

TEST_CASE("Test multi-block conversion")
{
	ColourRGB c_refColour0(0, 0, 0);
	ColourRGB c_refColour1(255, 255, 0);
	ColourRGB c_refColour2(0, 0, 255);
	ColourRGB c_refColour3(0, 255, 0);
	BlockCompressedImage testImage(16, 8);
	FillBlockWithColour(testImage, 0, 0, c_refColour0);
	FillBlockWithColour(testImage, 1, 0, c_refColour1);
	FillBlockWithColour(testImage, 2, 1, c_refColour2);
	FillBlockWithColour(testImage, 3, 1, c_refColour3);

	DXT1ToBitmapConverter converter;
	auto convertedImage = converter.Convert(testImage);
	REQUIRE(convertedImage.get() != nullptr);
	REQUIRE(convertedImage->GetWidthPixels() == 16);
	REQUIRE(convertedImage->GetHeightPixels() == 8);

	REQUIRE(BlockContainsValue(testImage, 0, 0, c_refColour0) == true);
	REQUIRE(BlockContainsValue(testImage, 1, 0, c_refColour1) == true);
	REQUIRE(BlockContainsValue(testImage, 2, 1, c_refColour2) == true);
	REQUIRE(BlockContainsValue(testImage, 3, 1, c_refColour3) == true);
}