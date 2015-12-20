#include "catch/catch.hpp"
#include "../ImgLib/raw_file_io.h"
#include "../ImgLib/raw_file_buffer.h"
#include "../ImgLib/bitmap_file_writer.h"
#include "../ImgLib/bitmap_file_reader.h"
#include "../ImgLib/image.h"

TEST_CASE("Write image data to file buffer")
{
	RawFileBuffer dataBuffer;
	Image testImageData(16, 16);
	BitmapFileWriter fileWriter;
	REQUIRE(fileWriter.WriteBitmapFile(testImageData, dataBuffer) == true);
	REQUIRE(dataBuffer.GetContents() != nullptr);
	REQUIRE(dataBuffer.GetBufferSize() == 822);		// 16x16 24 bit bmp should be 822 bytes
}

TEST_CASE("Don't write empty image")
{
	RawFileBuffer dataBuffer;
	Image testImageData(0, 8);
	BitmapFileWriter fileWriter;
	REQUIRE(fileWriter.WriteBitmapFile(testImageData, dataBuffer) == false);
}

TEST_CASE("Load / Write / Load cycle")
{
	std::unique_ptr<Image> sourceImage;
	RawFileBuffer sourceImageOriginalBuffer;
	RawFileBuffer sourceImageWroteToBuffer;

	{	// Load source image for testing
		RawFileBufferReader fileReader;
		REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16_24bit.bmp", sourceImageOriginalBuffer) == true);
		BitmapFileReader bmpFileReader;
		sourceImage = bmpFileReader.ExtractImage(sourceImageOriginalBuffer);
		REQUIRE(sourceImage.get() != nullptr);
	}

	{	// Write loaded image back to a new file data buffer
		BitmapFileWriter fileWriter;
		REQUIRE(fileWriter.WriteBitmapFile(*sourceImage.get(), sourceImageWroteToBuffer) == true);
		REQUIRE(sourceImageWroteToBuffer.GetContents() != nullptr);
	}

	{	// Read back from the saved buffer, the images should match
		BitmapFileReader bmpFileReader;
		std::unique_ptr<Image> savedImage;
		savedImage = bmpFileReader.ExtractImage(sourceImageWroteToBuffer);
		REQUIRE(sourceImage.get() != nullptr);
		REQUIRE(sourceImage->GetWidthPixels() == savedImage->GetWidthPixels());
		REQUIRE(sourceImage->GetHeightPixels() == savedImage->GetHeightPixels());
		for (uint32_t x = 0; x < sourceImage->GetWidthPixels(); ++x)
		{
			for (uint32_t y = 0; y < sourceImage->GetWidthPixels(); ++y)
			{
				ColourRGB sourcePixel, savedPixel;
				sourceImage->GetPixelColour(x, y, sourcePixel);
				sourceImage->GetPixelColour(x, y, savedPixel);
				REQUIRE(sourcePixel == savedPixel);
			}
		}
	}
}