#include "catch/catch.hpp"
#include "16bit_colour_test_helpers.h"
#include "../ImgLib/image.h"
#include "../ImgLib/block_compressed_image.h"
#include "../ImgLib/bitmap_to_dxt1_converter.h"
#include "../ImgLib/compress_block_minmax.h"

class BlockCompressEmptyMock : public IDXT1BlockRefColourCalculator
{
public:
	virtual ~BlockCompressEmptyMock() {}
	void CalculateBlockRefColours(const Image& sourceImg, uint32_t blockX, uint32_t blockY, ColourRGB& ref0, ColourRGB& ref1)
	{
		// do nothing
	}
};

TEST_CASE("Dont compress empty images")
{
	Image empty(0, 0);
	BlockCompressEmptyMock compression;
	BitmapToDXT1Converter converter;
	auto newImg = converter.Convert(empty, compression);
	REQUIRE(newImg == nullptr);
}

TEST_CASE("Dont compress images with bad width")
{
	Image srcImg(23, 32);
	BlockCompressEmptyMock compression;
	BitmapToDXT1Converter converter;
	auto newImg = converter.Convert(srcImg, compression);
	REQUIRE(newImg == nullptr);
}

TEST_CASE("Dont compress images with bad height")
{
	Image srcImg(64, 10);
	BlockCompressEmptyMock compression;
	BitmapToDXT1Converter converter;
	auto newImg = converter.Convert(srcImg, compression);
	REQUIRE(newImg == nullptr);
}

TEST_CASE("Create image of correct dimensions")
{
	Image srcImg(128, 256);
	BlockCompressEmptyMock compression;
	BitmapToDXT1Converter converter;
	auto newImg = converter.Convert(srcImg, compression);
	REQUIRE(newImg != nullptr);
	REQUIRE(newImg->GetWidthPixels() == 128);
	REQUIRE(newImg->GetHeightPixels() == 256);
	REQUIRE(newImg->GetWidthBlocks() == 32);
	REQUIRE(newImg->GetHeightBlocks() == 64);
}

TEST_CASE("Block compress solid colour with minmax")
{
	Image srcImage(8, 8);
	for (uint32_t x = 0; x < 8; ++x)
	{
		srcImage.SetPixelColour(x, 0, ColourRGB(255, 0, 0));
	}
	for (uint32_t x = 0; x < 8; ++x)
	{
		srcImage.SetPixelColour(x,6, ColourRGB(128, 0, 128));
	}

	CompressBlocksMinMax compressMinMax;
	BitmapToDXT1Converter converter;
	auto newImg = converter.Convert(srcImage, compressMinMax);
	
	ColourRGB testColour;
	for (uint32_t x = 0; x < 8; ++x)
	{
		newImg->GetPixelColour(x, 7, testColour);
		REQUIRE( APPROX_MATCH_COLOUR(testColour,ColourRGB(255, 0, 0)) );
	}

	for (uint32_t x = 0; x < 8; ++x)
	{
		newImg->GetPixelColour(x, 1, testColour);
		REQUIRE( APPROX_MATCH_COLOUR(testColour, ColourRGB(128, 0, 128)) );
	}
}