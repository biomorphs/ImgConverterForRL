#include "command_line_parameters.h"
#include "io_wrappers.h"

#include "../ImgLib/bitmap_to_dxt1_converter.h"
#include "../ImgLib/compress_block_minmax.h"
#include "../ImgLib/dxt1_to_bitmap_conversion.h"
#include "../ImgLib/block_compressed_image.h"
#include "../ImgLib/image.h"

#include <iostream>
#include <memory>

// Program Usage:
// ImgConverter <sourcefile> <destfile>

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

std::unique_ptr<IDXT1BlockRefColourCalculator> SelectDXTCompressor(CommandLineParameters::CompressionMethod type)
{
	switch (type)
	{
	case CommandLineParameters::CompressionMethod::UseMinMax:
		return std::make_unique<CompressBlocksMinMax>();
		break;
	default:
		return nullptr;
	};
}

bool ConvertBitmapToDDS(const std::string& source, const std::string& dest, CommandLineParameters::CompressionMethod compression)
{
	std::unique_ptr<Image> srcImage = LoadBitmapFromFile(source);
	if (srcImage == nullptr)
	{
		std::cout << "Failed to load source file '" << source << "'";
		return false;
	}

	auto compressor = SelectDXTCompressor(compression);
	BitmapToDXT1Converter converter;
	auto imgResult = converter.Convert(*srcImage, *compressor);
	if (imgResult == nullptr)
	{
		std::cout << "Failed to convert image from bmp to dds";
		return false;
	}
	if (!WriteImageToDXT1File(imgResult, dest))
	{
		std::cout << "Failed to write destination file '" << dest << "'";
		return false;
	}

	return true;
}

bool ConvertDDSToBitmap(const std::string& source, const std::string& dest)
{
	std::unique_ptr<BlockCompressedImage> srcImage = LoadDXT1FromFile(source);
	if (srcImage == nullptr)
	{
		std::cout << "Failed to load source file '" << source << "'";
		return false;
	}

	DXT1ToBitmapConverter converter;
	auto imgResult = converter.Convert(*srcImage);
	if (imgResult == nullptr)
	{
		std::cout << "Failed to convert image from dds to bmp";
		return false;
	}
	if (!WriteImageToBitmapFile(imgResult, dest))
	{
		std::cout << "Failed to write destination file '" << dest << "'";
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
	const auto srcPath = commandLineParams.GetSourcePath();
	const auto destPath = commandLineParams.GetDestinationPath();
	const auto compressionMethod = commandLineParams.GetCompressorMethod();
	if (srcFiletype == CommandLineParameters::FileType::Bitmap &&
		destFiletype == CommandLineParameters::FileType::DDS)
	{
		if (!ConvertBitmapToDDS(srcPath, destPath, compressionMethod))
		{
			return 1;
		}
	}
	else if (srcFiletype == CommandLineParameters::FileType::DDS &&
		destFiletype == CommandLineParameters::FileType::Bitmap)
	{
		if (!ConvertDDSToBitmap(srcPath, destPath))
		{
			return 1;
		}
	}
	else
	{
		std::cout << "Cannot convert these types. Source / dest must be either .bmp or .dds";
		return 1;
	}

    return 0;
}

