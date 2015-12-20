#include "catch/catch.hpp"
#include "../ImgLib/image.h"

TEST_CASE("Image created with correct width / height")
{
	Image img(16, 32);
	REQUIRE(img.GetWidthPixels() == 16);
	REQUIRE(img.GetHeightPixels() == 32);
}

TEST_CASE("Image created has buffer data")
{
	Image img(8, 8);
	ColourRGB pixelColour;
	REQUIRE_NOTHROW(img.GetPixelColour(0,0, pixelColour));
	REQUIRE_NOTHROW(img.GetPixelColour(8,8, pixelColour));
}

TEST_CASE("Set / get valid pixel values")
{
	Image img(8, 8);
	ColourRGB pixelColour;

	img.SetPixelColour(0, 0, ColourRGB(255, 0, 0));
	img.GetPixelColour(0, 0, pixelColour);
	REQUIRE(pixelColour == ColourRGB(255, 0, 0));

	img.SetPixelColour(4, 3, ColourRGB(0, 128, 128));
	img.GetPixelColour(4, 3, pixelColour);
	REQUIRE(pixelColour == ColourRGB(0, 128, 128));

	img.SetPixelColour(6, 2, ColourRGB(0, 0, 42));
	img.GetPixelColour(6, 2, pixelColour);
	REQUIRE(pixelColour == ColourRGB(0, 0, 42));

	SECTION("Invalid coords should not crash")
	{
		REQUIRE_NOTHROW(img.SetPixelColour(9, 0, ColourRGB(255, 255, 255)));
		REQUIRE_NOTHROW(img.SetPixelColour(0, 9, ColourRGB(255, 255, 255)));
		ColourRGB badData;
		REQUIRE_NOTHROW(img.GetPixelColour(9, 9, badData));
	}
}