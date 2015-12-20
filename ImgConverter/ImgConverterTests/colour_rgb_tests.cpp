#include "catch/catch.hpp"
#include "../ImgLib/colour_rgb.h"

TEST_CASE("colour constructors/get/set")
{
	ColourRGB black(0, 0, 0);
	ColourRGB red(255, 0, 0);
	ColourRGB green(0, 255, 0);
	ColourRGB white(255, 255, 255);
	ColourRGB darkBlue(0, 0, 64);

	SECTION("Black is black")
	{
		REQUIRE(black.GetRed() == 0);
		REQUIRE(black.GetGreen() == 0);
		REQUIRE(black.GetBlue() == 0);
	}

	SECTION("Red is not green")
	{
		REQUIRE(red != green);
	}

	SECTION("White is really white")
	{
		REQUIRE(white == ColourRGB(255, 255, 255));
	}

	SECTION("Copy")
	{
		auto redCopy = red;
		REQUIRE(redCopy == red);
		REQUIRE(redCopy != green);
	}

	SECTION("Set r / g / b")
	{
		ColourRGB col(0, 0, 0);
		col.SetRed(128);
		col.SetGreen(64);
		col.SetBlue(100);
		REQUIRE(col.GetRed() == 128);
		REQUIRE(col.GetGreen() == 64);
		REQUIRE(col.GetBlue() == 100);
	}

	SECTION("Set from RGB buffer data")
	{
		uint8_t rgbData[] = { 32, 64, 128 };
		ColourRGB colour(rgbData);
		REQUIRE(colour.GetRed() == 32);
		REQUIRE(colour.GetGreen() == 64);
		REQUIRE(colour.GetBlue() == 128);
	}

	SECTION("Set RGB buffer from colour")
	{
		uint8_t rgbData[] = { 0, 0, 0 };
		ColourRGB colour(50, 80, 39);
		colour.ToRGBBuffer(rgbData);

		REQUIRE(rgbData[0] == 50);
		REQUIRE(rgbData[1] == 80);
		REQUIRE(rgbData[2] == 39);
	}

	SECTION("to floating point")
	{
		REQUIRE(red.GetRedAsFloat() == Approx(1.0f));
		REQUIRE(red.GetGreenAsFloat() == Approx(0.0f));
		REQUIRE(red.GetBlueAsFloat() == Approx(0.0f));
		REQUIRE(darkBlue.GetBlueAsFloat() == Approx(64.0f / 255.0f));
	}

	SECTION("Quantise floats to rgba8")
	{
		float r = 32.0f / 255.0f, g = 28.0f / 255.0f, b = 15.0f / 255.0f;
		ColourRGB colour(r, g, b);
		REQUIRE(colour.GetRed() == 32);
		REQUIRE(colour.GetGreen() == 28);
		REQUIRE(colour.GetBlue() == 15);
	}
}