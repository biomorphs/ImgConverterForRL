#ifndef IMGCONVERTER_COMPRESS_BLOCK_MINMAX_INCLUDED
#define IMGCONVERTER_COMPRESS_BLOCK_MINMAX_INCLUDED

#include "image_to_block_compressed.h"

class CompressBlocksMinMax : public IBlockCompression
{
public:
	virtual ~CompressBlocksMinMax() { }
	void CalculateBlockRefColours(const Image& sourceImg, uint32_t blockX, uint32_t blockY, ColourRGB& ref0, ColourRGB& ref1);
};

#endif