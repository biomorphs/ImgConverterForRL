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