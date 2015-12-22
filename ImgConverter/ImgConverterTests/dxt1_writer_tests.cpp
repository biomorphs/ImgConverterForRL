#include "catch/catch.hpp"
#include "../ImgLib/raw_file_io.h"
#include "../ImgLib/raw_file_buffer.h"
#include "../ImgLib/dxt1_file_writer.h"
#include "../ImgLib/dxt1_file_reader.h"
#include "../ImgLib/block_compressed_image.h"

TEST_CASE("Write empty image to buffer")
{
	RawFileBuffer dataBuffer;
	BlockCompressedImage testImageData(16, 16);
	DXT1FileWriter fileWriter;
	REQUIRE(fileWriter.WriteFile(testImageData, dataBuffer) == true);
	REQUIRE(dataBuffer.GetContents() != nullptr);
	REQUIRE(dataBuffer.GetBufferSize() == 256);		// 16x16 dxt1 file should be 256 bytes
}

TEST_CASE("Don't write empty dxt image")
{
	RawFileBuffer dataBuffer;
	BlockCompressedImage testImageData(0, 8);
	DXT1FileWriter fileWriter;
	REQUIRE(fileWriter.WriteFile(testImageData, dataBuffer) == false);
}

TEST_CASE("Load / Write / Load dxt1 cycle")
{
	std::unique_ptr<BlockCompressedImage> sourceImage;
	RawFileBuffer sourceImageOriginalBuffer;
	RawFileBuffer sourceImageWroteToBuffer;

	{	// Load source image for testing
		RawFileBufferReader fileReader;
		REQUIRE(fileReader.LoadFromFile("../TestData/TestImg16x16.dds", sourceImageOriginalBuffer) == true);
		DXT1FileReader dxtFileReader;
		sourceImage = dxtFileReader.ExtractImage(sourceImageOriginalBuffer);
		REQUIRE(sourceImage.get() != nullptr);
	}

	{	// Write loaded image back to a new file data buffer
		DXT1FileWriter fileWriter;
		REQUIRE(fileWriter.WriteFile(*sourceImage.get(), sourceImageWroteToBuffer) == true);
		REQUIRE(sourceImageWroteToBuffer.GetContents() != nullptr);
	}

	{	// Read back from the saved buffer, the images should match
		DXT1FileReader bmpFileReader;
		std::unique_ptr<BlockCompressedImage> savedImage;
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