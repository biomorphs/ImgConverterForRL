#include "io_wrappers.h"

#include "../ImgLib/block_compressed_image.h"
#include "../ImgLib/image.h"
#include "../ImgLib/dxt1_file_reader.h"
#include "../ImgLib/dxt1_file_writer.h"
#include "../ImgLib/bitmap_file_reader.h"
#include "../ImgLib/bitmap_file_writer.h"
#include "../ImgLib/raw_file_buffer.h"
#include "../ImgLib/raw_file_io.h"

std::unique_ptr<Image> LoadBitmapFromFile(const std::string& path)
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	if (!fileReader.LoadFromFile(path, dataBuffer))
	{
		return nullptr;
	}
	BitmapFileReader bmpFileReader;
	return bmpFileReader.ExtractImage(dataBuffer);
}

bool WriteImageToBitmapFile(const std::unique_ptr<Image>& source, const std::string& destPath)
{
	RawFileBuffer outputBuffer;
	BitmapFileWriter fileWriter;
	if (!fileWriter.WriteFile(*source.get(), outputBuffer))
	{
		return false;
	}
	RawFileBufferWriter bufferWriter;
	return bufferWriter.WriteTofile(destPath, outputBuffer);
}

std::unique_ptr<BlockCompressedImage> LoadDXT1FromFile(const std::string& path)
{
	RawFileBufferReader fileReader;
	RawFileBuffer dataBuffer;
	if (!fileReader.LoadFromFile(path, dataBuffer))
	{
		return nullptr;
	}
	DXT1FileReader dxtFileReader;
	return dxtFileReader.ExtractImage(dataBuffer);
}

bool WriteImageToDXT1File(const std::unique_ptr<BlockCompressedImage>& source, const std::string& destPath)
{
	RawFileBuffer outputBuffer;
	DXT1FileWriter fileWriter;
	if (!fileWriter.WriteFile(*source.get(), outputBuffer))
	{
		return false;
	}
	RawFileBufferWriter bufferWriter;
	return bufferWriter.WriteTofile(destPath, outputBuffer);
}