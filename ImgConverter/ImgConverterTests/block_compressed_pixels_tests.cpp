#include "catch/catch.hpp"
#include "16bit_colour_test_helpers.h"
#include "../ImgLib/colour_rgb.h"
#include "../ImgLib/block_compressed_pixels.h"

TEST_CASE("Block compressed pixel ref colours correct")
{
	BlockCompressedPixels pixels;
	pixels.SetRefColour1(ColourRGB(255, 0, 0));
	pixels.SetRefColour2(ColourRGB(0, 0, 255));
	REQUIRE(pixels.GetRefColour1() == ColourRGB(255, 0, 0).ToR5G6B5());
	REQUIRE(pixels.GetRefColour2() == ColourRGB(0, 0, 255).ToR5G6B5());
}

TEST_CASE("Block compressed pixel index set/get")
{
	BlockCompressedPixels pixels;
	pixels.SetPixelColourIndex(0, 0, 1);
	REQUIRE(pixels.GetPixelColourIndex(0, 0) == 1);

	pixels.SetPixelColourIndex(3, 0, 2);
	REQUIRE(pixels.GetPixelColourIndex(3, 0) == 2);

	pixels.SetPixelColourIndex(1, 2, 3);
	REQUIRE(pixels.GetPixelColourIndex(1, 2) == 3);

	pixels.SetPixelColourIndex(3, 3, 0);
	REQUIRE(pixels.GetPixelColourIndex(3, 3) == 0);
}

TEST_CASE("Block compressed pixels colour table generation")
{
	BlockCompressedPixels pixels;
	pixels.SetRefColour1(ColourRGB(255, 0, 0));
	pixels.SetRefColour2(ColourRGB(0, 0, 255));
	
	BlockCompressedPixels::ColourLUT lut(pixels);
	REQUIRE(APPROX_MATCH_QUANTISED(8, lut.GetColour(0).GetRed(), 255));
	REQUIRE(APPROX_MATCH_QUANTISED(4, lut.GetColour(0).GetGreen(), 0));
	REQUIRE(APPROX_MATCH_QUANTISED(8, lut.GetColour(0).GetBlue(), 0));

	REQUIRE(APPROX_MATCH_QUANTISED(8, lut.GetColour(1).GetRed(), 0));
	REQUIRE(APPROX_MATCH_QUANTISED(4, lut.GetColour(1).GetGreen(), 0));
	REQUIRE(APPROX_MATCH_QUANTISED(8, lut.GetColour(1).GetBlue(), 255));

	// 2/3*color_0 + 1/3*color_1
	REQUIRE(APPROX_MATCH_QUANTISED(8, lut.GetColour(2).GetRed(), 170));
	REQUIRE(APPROX_MATCH_QUANTISED(4, lut.GetColour(2).GetGreen(), 0));
	REQUIRE(APPROX_MATCH_QUANTISED(8, lut.GetColour(2).GetBlue(), 85));

	// 1/3*color_0 + 2/3*color_1
	REQUIRE(APPROX_MATCH_QUANTISED(8, lut.GetColour(3).GetRed(), 85));
	REQUIRE(APPROX_MATCH_QUANTISED(4, lut.GetColour(3).GetGreen(), 0));
	REQUIRE(APPROX_MATCH_QUANTISED(8, lut.GetColour(3).GetBlue(), 170));
}

TEST_CASE("Block compression closest colour tests")
{
	BlockCompressedPixels pixels;
	pixels.SetRefColour1(ColourRGB(255, 0, 0));
	pixels.SetRefColour2(ColourRGB(0, 0, 255));

	BlockCompressedPixels::ColourLUT lut(pixels);

	REQUIRE(lut.ClosestIndex(ColourRGB(255, 0, 0)) == 0);
	REQUIRE(lut.ClosestIndex(ColourRGB(0, 0, 255)) == 1);
	REQUIRE(lut.ClosestIndex(ColourRGB(220, 0, 32)) == 0);
	REQUIRE(lut.ClosestIndex(ColourRGB(16, 20, 230)) == 1);
	REQUIRE(lut.ClosestIndex(ColourRGB(100, 0, 0)) == 3);
	REQUIRE(lut.ClosestIndex(ColourRGB(200, 0, 180)) == 2);
}