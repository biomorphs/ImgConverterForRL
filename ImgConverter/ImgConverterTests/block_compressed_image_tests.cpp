#include "catch/catch.hpp"
#include "../ImgLib/block_compressed_image.h"

TEST_CASE("Block compressed image created")
{
	BlockCompressedImage img(32, 32);
	REQUIRE(img.GetWidthPixels() == 32);
	REQUIRE(img.GetHeightPixels() == 32);
	REQUIRE(img.GetWidthBlocks() == 8);
	REQUIRE(img.GetHeightBlocks() == 8);
}

TEST_CASE("Block compressed image has buffer allocated")
{
	BlockCompressedImage img(32, 32);
	REQUIRE(img.GetWidthPixels() == 32);
	REQUIRE(img.GetHeightPixels() == 32);
	REQUIRE(img.GetWidthBlocks() == 8);
	REQUIRE(img.GetHeightBlocks() == 8);
}

TEST_CASE("Fail access to bad block")
{
	BlockCompressedImage img(8, 8);
	auto blockToWrite = img.BlockAt(3, 0);
	REQUIRE(blockToWrite == nullptr);

	blockToWrite = img.BlockAt(1, 4);
	REQUIRE(blockToWrite == nullptr);
}

TEST_CASE("Block write/read")
{
	BlockCompressedImage img(8, 8);
	auto blockToWrite = img.BlockAt(1, 1);
	REQUIRE(blockToWrite != nullptr);
	blockToWrite->SetRefColour1(ColourRGB(255, 0, 255));
	const auto blockToRead = img.BlockAt(1, 1);
	REQUIRE(blockToRead == blockToWrite);
}

TEST_CASE("Pixel colour helper")
{
	BlockCompressedImage img(16, 16);
	auto blockToWrite = img.BlockAt(3, 2);
	REQUIRE(blockToWrite != nullptr);
	blockToWrite->SetRefColour1(ColourRGB(255, 0, 255));
	blockToWrite->SetRefColour2(ColourRGB(0, 255, 0));

	blockToWrite->SetPixelColourIndex(1, 2, 0);
	blockToWrite->SetPixelColourIndex(3, 3, 1);

	ColourRGB testColour;
	img.GetPixelColour(13, 10, testColour);
	REQUIRE(testColour == ColourRGB(255, 0, 255));

	img.GetPixelColour(15, 11, testColour);
	REQUIRE(testColour == ColourRGB(0, 255, 0));
}