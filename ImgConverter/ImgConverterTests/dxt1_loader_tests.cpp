#include "catch/catch.hpp"
#include "../ImgLib/raw_file_io.h"
#include "../ImgLib/raw_file_buffer.h"
#include "../ImgLib/dxt1_file_reader.h"
#include "../ImgLib/dxt1_file_header.h"
#include "../ImgLib/block_compressed_image.h"

TEST_CASE("Parse empty dds file")
{
	RawFileBuffer dataBuffer;
	DXT1FileReader fileReader;
	DXT1FileHeader imageHeader;
	REQUIRE(fileReader.ExtractHeader(dataBuffer, imageHeader) == false);
}

TEST_CASE("Parse invalid dds data")
{
	const uint8_t badData[] = { 85, 24, 12, 7, 2 };	// Some random data that should not match a bitmap header
	RawFileBuffer dataBuffer;
	dataBuffer.AppendBytes(badData, sizeof(badData));

	DXT1FileReader fileReader;
	DXT1FileHeader imageHeader;
	REQUIRE(fileReader.ExtractHeader(dataBuffer, imageHeader) == false);
}

TEST_CASE("Test DXT1 support from valid header")
{
	DXT1FileHeader dxt1Header;
	dxt1Header.m_ddsFileToken = c_ddsFileToken;
	dxt1Header.m_headerSize = c_dxt1HeaderSize;
	dxt1Header.m_flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;
	dxt1Header.m_heightPx = 16;
	dxt1Header.m_widthPx = 16;
	dxt1Header.m_pitchOrLinearSize = 256;
	dxt1Header.m_mipCount = 0;
	dxt1Header.m_pixelFormatSize = c_dxt1PixelFormatSize;
	dxt1Header.m_pixelFormatFlags = DDPF_FOURCC;
	dxt1Header.m_pixelFormatFourCC = c_dxt1FourCC;
	dxt1Header.m_capsFlags = DDSCAPS_TEXTURE;

	DXT1FileReader fileReader;
	REQUIRE(fileReader.IsFormatSupported(dxt1Header) == true);
}

TEST_CASE("Test unsupported dds formats")
{
	DXT1FileHeader dxt1Header;
	dxt1Header.m_ddsFileToken = c_ddsFileToken;
	dxt1Header.m_headerSize = c_dxt1HeaderSize;
	dxt1Header.m_flags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT | DDSD_LINEARSIZE;
	dxt1Header.m_heightPx = 16;
	dxt1Header.m_widthPx = 16;
	dxt1Header.m_pitchOrLinearSize = 256;
	dxt1Header.m_mipCount = 0;
	dxt1Header.m_pixelFormatSize = c_dxt1PixelFormatSize;
	dxt1Header.m_pixelFormatFlags = DDPF_FOURCC;
	dxt1Header.m_pixelFormatFourCC = c_dxt1FourCC;
	dxt1Header.m_capsFlags = DDSCAPS_TEXTURE;

	DXT1FileReader fileReader;

	SECTION("Bad flags")
	{
		dxt1Header.m_flags = 0;
		REQUIRE(fileReader.IsFormatSupported(dxt1Header) == false);
	}

	SECTION("Bad height")
	{
		dxt1Header.m_heightPx = 0;
		REQUIRE(fileReader.IsFormatSupported(dxt1Header) == false);
	}

	SECTION("Bad width")
	{
		dxt1Header.m_widthPx = 0;
		REQUIRE(fileReader.IsFormatSupported(dxt1Header) == false);
	}

	SECTION("No pitch")
	{
		dxt1Header.m_pitchOrLinearSize = 0;
		REQUIRE(fileReader.IsFormatSupported(dxt1Header) == false);
	}

	SECTION("Wrong format")
	{
		dxt1Header.m_pixelFormatFourCC = '3TXD';
		REQUIRE(fileReader.IsFormatSupported(dxt1Header) == false);
	}

	SECTION("Wrong format")
	{
		dxt1Header.m_pixelFormatFlags = 0;
		REQUIRE(fileReader.IsFormatSupported(dxt1Header) == false);
	}
}

TEST_CASE("Parse valid 16x16 dxt1 header")
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16.dds", dataBuffer) == true);

	DXT1FileReader dxt1Reader;
	DXT1FileHeader dxt1Header;
	REQUIRE(dxt1Reader.ExtractHeader(dataBuffer, dxt1Header) == true);
	REQUIRE(dxt1Header.m_widthPx == 16);
	REQUIRE(dxt1Header.m_heightPx == 16);
	REQUIRE(dxt1Reader.IsFormatSupported(dxt1Header) == true);
}

TEST_CASE("Reject valid dxt3 file")
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16_dxt3.dds", dataBuffer) == true);

	DXT1FileReader dxt1Reader;
	DXT1FileHeader dxt1Header;
	REQUIRE(dxt1Reader.ExtractHeader(dataBuffer, dxt1Header) == true);
	REQUIRE(dxt1Reader.IsFormatSupported(dxt1Header) == false);
}

TEST_CASE("Load valid 16x16 dxt1 file into image storage")
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16.dds", dataBuffer) == true);

	DXT1FileReader dxtFileReader;
	auto loadedImage = dxtFileReader.ExtractImage(dataBuffer);
	REQUIRE(loadedImage.get() != nullptr);
	REQUIRE(loadedImage->GetWidthPixels() == 16);
	REQUIRE(loadedImage->GetHeightPixels() == 16);
	REQUIRE(loadedImage->GetWidthBlocks() == 4);
	REQUIRE(loadedImage->GetHeightBlocks() == 4);

	SECTION("Test known values using direct pixel lookup")
	{
		ColourRGB testColour;

		ColourRGB tl, tr, bl, br;
		loadedImage->GetPixelColour(0, 15, tl);
		loadedImage->GetPixelColour(15, 15, tr);
		loadedImage->GetPixelColour(0, 0, bl);
		loadedImage->GetPixelColour(15, 0, br);

		// top-left = red
		loadedImage->GetPixelColour(0, 15, testColour);
		REQUIRE(testColour == ColourRGB(255, 0, 0));

		// top-right = green
		loadedImage->GetPixelColour(15, 15, testColour);
		REQUIRE(testColour == ColourRGB(0, 255, 0));

		// Bottom-left = blue
		loadedImage->GetPixelColour(0, 0, testColour);
		REQUIRE(testColour == ColourRGB(0, 0, 255));

		// Bottom-right = white
		loadedImage->GetPixelColour(15, 0, testColour);
		REQUIRE(testColour == ColourRGB(255, 255, 255));
	}
}