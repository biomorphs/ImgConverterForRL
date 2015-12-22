#ifndef IMGCONVERTER_IMAGE_TO_BLOCK_COMPRESSED_INCLUDED
#define IMGCONVERTER_IMAGE_TO_BLOCK_COMPRESSED_INCLUDED

#include <memory>

class BlockCompressedImage;
class BlockCompressedPixels;
class Image;
class ColourRGB;

// Interface for calculating LUT ref colours for blocks
class IBlockCompression
{
public:
	virtual ~IBlockCompression() { }
	virtual void CalculateBlockRefColours(const Image& sourceImg, uint32_t blockX, uint32_t blockY, ColourRGB& ref0, ColourRGB& ref1) = 0;
};

class ImageToBlockCompressedConverter
{
public:
	ImageToBlockCompressedConverter();
	~ImageToBlockCompressedConverter();

	std::unique_ptr<BlockCompressedImage> ConvertImage(const Image& source, IBlockCompression& compressor);

private:
	void BuildPixelIndicesForBlock(const Image& source, uint32_t blockX, uint32_t blockY, BlockCompressedPixels& target);
};

#endif