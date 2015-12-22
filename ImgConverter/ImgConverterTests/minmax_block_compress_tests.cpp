#include "catch/catch.hpp"
#include "../ImgLib/compress_block_minmax.h"
#include "../ImgLib/image.h"
#include "../ImgLib/block_compressed_pixels.h"

TEST_CASE("Min/Max are correct")
{
	Image testImage(8, 8);

	// Write rows of colour
	for (uint32_t x = 4; x < 8; ++x)
	{
		testImage.SetPixelColour(x, 0, ColourRGB(32, 64, 5));
	}
	for (uint32_t x = 4; x < 8; ++x)
	{
		testImage.SetPixelColour(x, 1, ColourRGB(16, 128, 40));
	}
	for (uint32_t x = 4; x < 8; ++x)
	{
		testImage.SetPixelColour(x, 2, ColourRGB(200, 12, 80));
	}
	for (uint32_t x = 4; x < 8; ++x)
	{
		testImage.SetPixelColour(x, 3, ColourRGB(100, 2, 220));
	}	
	
	CompressBlocksMinMax minMax;
	ColourRGB minColour, maxColour;
	minMax.CalculateBlockRefColours(testImage, 1, 1, minColour, maxColour);
	
	REQUIRE(minColour.GetRed() == 16);
	REQUIRE(minColour.GetGreen() == 2);
	REQUIRE(minColour.GetBlue() == 5);

	REQUIRE(maxColour.GetRed() == 200);
	REQUIRE(maxColour.GetGreen() == 128);
	REQUIRE(maxColour.GetBlue() == 220);
}