#include "compress_block_minmax.h"
#include "image.h"

void CompressBlocksMinMax::CalculateBlockRefColours(const Image& sourceImg, uint32_t blockX, uint32_t blockY, ColourRGB& minColour, ColourRGB& maxColour)
{
	ColourRGB minColourAccum(255,255,255), maxColourAccum(0,0,0);
	const uint32_t c_startX = blockX << 2;
	const uint32_t c_startY = blockY << 2;
	const uint32_t c_endX = c_startX + 4;
	const uint32_t c_endY = c_startY + 4;
	const uint32_t c_sourceImgMaxY = sourceImg.GetHeightPixels();
	for (uint32_t pY = c_startY; pY < c_endY; ++pY)
	{
		for (uint32_t pX = c_startX; pX < c_endX; ++pX)
		{
			const uint32_t flippedY = c_sourceImgMaxY - 1 - pY;
			ColourRGB onePixel;
			sourceImg.GetPixelColour(pX, flippedY, onePixel);
			minColourAccum = ColourRGB::Min(minColourAccum, onePixel);
			maxColourAccum = ColourRGB::Max(maxColourAccum, onePixel);
		}
	}
	minColour = minColourAccum;
	maxColour = maxColourAccum;
}