#include "catch/catch.hpp"
#include "16bit_colour_test_helpers.h"
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

	SECTION("R5G6B5 conversions")
	{
		const uint16_t c_redAsR5G6B5 = MAKE_R5G6B5(255, 0, 0);
		const uint16_t c_greenAsR5G6B5 = MAKE_R5G6B5(0, 255, 0);
		const uint16_t c_blueAsR5G6B5 = MAKE_R5G6B5(0, 0, 255);

		SECTION("Quantisation")
		{
			ColourRGB darkRed(63, 0, 0);
			uint16_t darkRed16Bit = MAKE_R5G6B5(63, 0, 0);
			REQUIRE(darkRed.ToR5G6B5() == darkRed16Bit);

			ColourRGB lightGreen(63, 192, 63);
			uint16_t lightGreen16Bit = MAKE_R5G6B5(63, 192, 63);
			REQUIRE(lightGreen.ToR5G6B5() == lightGreen16Bit);
		}

		SECTION("expansion")
		{
			ColourRGB redFrom16Bit(c_redAsR5G6B5);
			REQUIRE(redFrom16Bit.GetRed() == 255);
			REQUIRE(redFrom16Bit.GetGreen() == 0);
			REQUIRE(redFrom16Bit.GetBlue() == 0);

			ColourRGB greenFrom16Bit(c_greenAsR5G6B5);
			REQUIRE(greenFrom16Bit.GetRed() == 0);
			REQUIRE(greenFrom16Bit.GetGreen() == 255);
			REQUIRE(greenFrom16Bit.GetBlue() == 0);

			ColourRGB blueFrom16Bit(c_blueAsR5G6B5);
			REQUIRE(blueFrom16Bit.GetRed() == 0);
			REQUIRE(blueFrom16Bit.GetGreen() == 0);
			REQUIRE(blueFrom16Bit.GetBlue() == 255);
			
			// Max error from 1 byte -> 5 bit conversion = 8
			// Max error from 1 byte -> 6 bit conversion = 4
			ColourRGB lightGreenFrom16bit(MAKE_R5G6B5(64,192,64));
			REQUIRE(APPROX_MATCH_QUANTISED(8, lightGreenFrom16bit.GetRed(), 64));
			REQUIRE(APPROX_MATCH_QUANTISED(4, lightGreenFrom16bit.GetGreen(), 192));
			REQUIRE(APPROX_MATCH_QUANTISED(8, lightGreenFrom16bit.GetBlue(), 64));

			ColourRGB darkerRed(MAKE_R5G6B5(32, 0, 0));
			REQUIRE(APPROX_MATCH_QUANTISED(8, darkerRed.GetRed(), 32));
			REQUIRE(APPROX_MATCH_QUANTISED(4, darkerRed.GetGreen(), 0));
			REQUIRE(APPROX_MATCH_QUANTISED(8, darkerRed.GetBlue(), 0));
		}
	}
}