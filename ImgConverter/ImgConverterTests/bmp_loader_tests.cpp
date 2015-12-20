#include "catch/catch.hpp"
#include "../ImgLib/raw_file_io.h"
#include "../ImgLib/raw_file_buffer.h"
#include "../ImgLib/bitmap_file_reader.h"
#include "../ImgLib/image.h"

TEST_CASE("Parse empty file buffer")
{
	RawFileBuffer dataBuffer;
	BitmapFileReader bmpFileReader;
	BitmapFileHeader bmpHeader;
	REQUIRE(bmpFileReader.ExtractHeader(dataBuffer, bmpHeader) == false);
}

TEST_CASE("Parse invalid data")
{
	const uint8_t badData[] = { 85, 24, 12, 7, 2 };	// Some random data that should not match a bitmap header
	RawFileBuffer dataBuffer;
	dataBuffer.AppendBytes(badData, sizeof(badData));

	BitmapFileReader bmpFileReader;
	BitmapFileHeader bmpHeader;
	REQUIRE(bmpFileReader.ExtractHeader(dataBuffer, bmpHeader) == false);
}

TEST_CASE("Test valid header data")
{
	BitmapFileHeader testHeader;
	testHeader.m_fileTypeToken = 0x4d42;
	testHeader.m_totalFileSize = 0;
	testHeader.m_reserved = 0;
	testHeader.m_dataOffset = sizeof(BitmapFileHeader);
	testHeader.m_infoHeaderSize = 40;
	testHeader.m_imgWidthPx = 8;
	testHeader.m_imgHeightPx = 8;
	testHeader.m_planeCount = 1;
	testHeader.m_colourBitsPerPx = 24;
	testHeader.m_compressionType = 0;
	testHeader.m_imageSizeBytes = 192;
	testHeader.m_pixelsPerMeterX = 1024;
	testHeader.m_pixelsPerMeterY = 1024;
	testHeader.m_colourIndicesUsed = 0;
	testHeader.m_colourIndicesImportant = 0;
	BitmapFileReader fileReader;
	REQUIRE(fileReader.IsBitmapFormatSupported(testHeader) == true);
}

TEST_CASE("Test invalid header data")
{
	BitmapFileHeader testHeader;
	testHeader.m_fileTypeToken = 0x4d42;
	testHeader.m_totalFileSize = 0;
	testHeader.m_reserved = 0;
	testHeader.m_dataOffset = sizeof(BitmapFileHeader);
	testHeader.m_infoHeaderSize = 40;
	testHeader.m_imgWidthPx = 8;
	testHeader.m_imgHeightPx = 8;
	testHeader.m_planeCount = 1;
	testHeader.m_colourBitsPerPx = 24;
	testHeader.m_compressionType = 0;
	testHeader.m_imageSizeBytes = 192;
	testHeader.m_pixelsPerMeterX = 1024;
	testHeader.m_pixelsPerMeterY = 1024;
	testHeader.m_colourIndicesUsed = 0;
	testHeader.m_colourIndicesImportant = 0;

	BitmapFileReader bmpFileReader;

	SECTION("Bad data offset")
	{
		testHeader.m_dataOffset = 0;
		REQUIRE(bmpFileReader.IsBitmapFormatSupported(testHeader) == false);
	}

	SECTION("Unsupported bmp type")
	{
		testHeader.m_infoHeaderSize = 0;
		REQUIRE(bmpFileReader.IsBitmapFormatSupported(testHeader) == false);
	}

	SECTION("Unsupported compression")
	{
		testHeader.m_compressionType = 1;
		REQUIRE(bmpFileReader.IsBitmapFormatSupported(testHeader) == false);
	}

	SECTION("Unsupported pixel format")
	{
		testHeader.m_colourBitsPerPx = 8;
		REQUIRE(bmpFileReader.IsBitmapFormatSupported(testHeader) == false);
	}
}

TEST_CASE("Parse valid 24bit 16x16 bmp file header")
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16_24bit.bmp", dataBuffer) == true);

	BitmapFileReader bmpFileReader;
	BitmapFileHeader bmpHeader;

	REQUIRE(bmpFileReader.ExtractHeader(dataBuffer, bmpHeader) == true);
	
	SECTION("Test header for known values")
	{
		REQUIRE(bmpHeader.m_fileTypeToken == 0x4d42);
		REQUIRE(bmpHeader.m_colourBitsPerPx == 24);
		REQUIRE(bmpHeader.m_imgWidthPx == 16);
		REQUIRE(bmpHeader.m_imgHeightPx == 16);
	}

	SECTION("Validate header with reader")
	{
		REQUIRE(bmpFileReader.IsBitmapFormatSupported(bmpHeader) == true);
	}
}

TEST_CASE("Parse unsupported bitmap header")
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16_8bit.bmp", dataBuffer) == true);

	BitmapFileReader bmpFileReader;
	BitmapFileHeader bmpHeader;

	REQUIRE(bmpFileReader.ExtractHeader(dataBuffer, bmpHeader) == true);
	REQUIRE(bmpFileReader.IsBitmapFormatSupported(bmpHeader) == false);
}

TEST_CASE("Load valid 24bit 16x16 bmp file into image storage")
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16_24bit.bmp", dataBuffer) == true);

	BitmapFileReader bmpFileReader;
	auto loadedImage = bmpFileReader.ExtractImage(dataBuffer);
	REQUIRE(loadedImage.get() != nullptr);
	REQUIRE(loadedImage->GetWidthPixels() == 16);
	REQUIRE(loadedImage->GetHeightPixels() == 16);

	SECTION("Test known values")
	{
		ColourRGB testColour;

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

TEST_CASE("Load unsupported bitmap format")
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16_8bit.bmp", dataBuffer) == true);

	BitmapFileReader bmpFileReader;
	auto loadedImage = bmpFileReader.ExtractImage(dataBuffer);
	REQUIRE(loadedImage.get() == nullptr);
}

TEST_CASE("Load invalid file as bitmap")
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/Test_240_128.dds", dataBuffer) == true);

	BitmapFileReader bmpFileReader;
	auto loadedImage = bmpFileReader.ExtractImage(dataBuffer);
	REQUIRE(loadedImage.get() == nullptr);
}