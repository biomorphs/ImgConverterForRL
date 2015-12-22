#include "command_line_parameters.h"
#include "../ImgLib/image_to_block_compressed.h"
#include "../ImgLib/compress_block_minmax.h"
#include "../ImgLib/dxt1_to_bitmap_conversion.h"
#include "../ImgLib/block_compressed_image.h"
#include "../ImgLib/dxt1_file_reader.h"
#include "../ImgLib/dxt1_file_writer.h"
#include "../ImgLib/bitmap_file_reader.h"
#include "../ImgLib/bitmap_file_writer.h"
#include "../ImgLib/raw_file_buffer.h"
#include "../ImgLib/raw_file_io.h"
#include "../ImgLib/image.h"
#include <iostream>
#include <memory>

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
	if (!fileWriter.WriteBitmapFile(*source.get(), outputBuffer))
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
	DXT1FileReader dxtFileReaderr;
	return dxtFileReaderr.ExtractImage(dataBuffer);
}

bool WriteImageToDXT1File(const std::unique_ptr<BlockCompressedImage>& source, const std::string& destPath)
{
	RawFileBuffer outputBuffer;
	DXT1FileWriter fileWriter;
	if (!fileWriter.WriteDXT1File(*source.get(), outputBuffer))
	{
		return false;
	}
	RawFileBufferWriter bufferWriter;
	return bufferWriter.WriteTofile(destPath, outputBuffer);
}

bool ProcessCommandLine(int argc, char *argv[], CommandLineParameters& params)
{
	if (params.ParseCommandLine(argc, argv) != CommandLineParameters::ParseResult::OK)
	{
		params.DisplayHelp();
		return false;
	}
	if (params.GetSourceFiletype() == CommandLineParameters::FileType::Unknown)
	{
		std::cout << "Source file-type is not a .bmp or .dds file!";
		return false;
	}
	if (params.GetDestinationFiletype() == CommandLineParameters::FileType::Unknown)
	{
		std::cout << "Destination file-type is not a .bmp or .dds file!";
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	CommandLineParameters commandLineParams;
	if (!ProcessCommandLine(argc, argv, commandLineParams))
	{
		return 1;
	}

	const auto srcFiletype = commandLineParams.GetSourceFiletype();
	const auto destFiletype = commandLineParams.GetDestinationFiletype();
	if (srcFiletype == CommandLineParameters::FileType::Bitmap && 
		destFiletype == CommandLineParameters::FileType::Bitmap)
	{
		std::unique_ptr<Image> srcImage = LoadBitmapFromFile(commandLineParams.GetSourcePath());
		if (srcImage == nullptr)
		{
			std::cout << "Failed to load source file '" << commandLineParams.GetSourcePath() << "'";
			return 1;
		}
		if (!WriteImageToBitmapFile(srcImage, commandLineParams.GetDestinationPath()))
		{
			std::cout << "Failed to write destination file '" << commandLineParams.GetDestinationPath() << "'";
			return 1;
		}
	}
	else if (srcFiletype == CommandLineParameters::FileType::DDS &&
		destFiletype == CommandLineParameters::FileType::DDS)
	{
		std::unique_ptr<BlockCompressedImage> srcImage = LoadDXT1FromFile(commandLineParams.GetSourcePath());
		if (srcImage == nullptr)
		{
			std::cout << "Failed to load source file '" << commandLineParams.GetSourcePath() << "'";
			return 1;
		}
		if (!WriteImageToDXT1File(srcImage, commandLineParams.GetDestinationPath()))
		{
			std::cout << "Failed to write destination file '" << commandLineParams.GetDestinationPath() << "'";
			return 1;
		}
	}
	else if (srcFiletype == CommandLineParameters::FileType::DDS &&
		destFiletype == CommandLineParameters::FileType::Bitmap)
	{
		std::unique_ptr<BlockCompressedImage> srcImage = LoadDXT1FromFile(commandLineParams.GetSourcePath());
		if (srcImage == nullptr)
		{
			std::cout << "Failed to load source file '" << commandLineParams.GetSourcePath() << "'";
			return 1;
		}
		DXT1ToBitmapConversion converter;
		auto imgResult = converter.Convert(*srcImage);
		if (imgResult == nullptr)
		{
			std::cout << "Failed to convert image from dds to bmp";
			return 1;
		}
		if (!WriteImageToBitmapFile(imgResult, commandLineParams.GetDestinationPath()))
		{
			std::cout << "Failed to write destination file '" << commandLineParams.GetDestinationPath() << "'";
			return 1;
		}
	}
	else if (srcFiletype == CommandLineParameters::FileType::Bitmap &&
		destFiletype == CommandLineParameters::FileType::DDS)
	{
		std::unique_ptr<Image> srcImage = LoadBitmapFromFile(commandLineParams.GetSourcePath());
		if (srcImage == nullptr)
		{
			std::cout << "Failed to load source file '" << commandLineParams.GetSourcePath() << "'";
			return 1;
		}
		ImageToBlockCompressedConverter converter;
		CompressBlocksMinMax minMaxSampler;
		auto imgResult = converter.ConvertImage(*srcImage, minMaxSampler);
		if (imgResult == nullptr)
		{
			std::cout << "Failed to convert image from bmp to dds";
			return 1;
		}
		if (!WriteImageToDXT1File(imgResult, commandLineParams.GetDestinationPath()))
		{
			std::cout << "Failed to write destination file '" << commandLineParams.GetDestinationPath() << "'";
			return 1;
		}
	}

    return 0;
}

