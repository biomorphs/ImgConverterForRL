#ifndef IMGCONVERTER_BLOCK_COMPRESSED_PIXELS_INL_INCLUDED
#define IMGCONVERTER_BLOCK_COMPRESSED_PIXELS_INL_INCLUDED

#include "utils.h"
#include "colour_rgb.h"

BlockCompressedPixels::BlockCompressedPixels()
	: m_rawData(0)
{
}

BlockCompressedPixels::BlockCompressedPixels(const BlockCompressedPixels& other)
{
	m_rawData = other.m_rawData;
}

BlockCompressedPixels::BlockCompressedPixels(BlockCompressedPixels&& other)
{
	m_rawData = other.m_rawData;
}

BlockCompressedPixels::~BlockCompressedPixels()
{
}

BlockCompressedPixels& BlockCompressedPixels::operator=(const BlockCompressedPixels& other)
{
	m_rawData = other.m_rawData;
	return *this;
}

BlockCompressedPixels& BlockCompressedPixels::operator=(BlockCompressedPixels&& other)
{
	m_rawData = other.m_rawData;
	return *this;
}

void BlockCompressedPixels::SetRefColour1(const ColourRGB& col)
{
	m_refColour1 = col.ToR5G6B5();
}

void BlockCompressedPixels::SetRefColour2(const ColourRGB& col)
{
	m_refColour2 = col.ToR5G6B5();
}

ColourRGB BlockCompressedPixels::GetRefColour1() const
{
	return ColourRGB(m_refColour1);
}

ColourRGB BlockCompressedPixels::GetRefColour2() const
{
	return ColourRGB(m_refColour2);
}

void BlockCompressedPixels::SetPixelColourIndex(uint32_t x, uint32_t y, uint32_t id)
{
	// truncate index to 2 bits and shift to correct position
	const uint32_t indexTrunc = id & 3;
	const uint32_t indexPosition = GetIndexShiftPosition(x, y);

	const uint32_t shiftedIndex = indexTrunc << indexPosition;
	const uint32_t writeMask = ~(0x3 << indexPosition);
	
	m_indices = (m_indices & writeMask) | shiftedIndex;
}

uint32_t BlockCompressedPixels::GetPixelColourIndex(uint32_t x, uint32_t y) const
{
	const uint32_t indexPosition = GetIndexShiftPosition(x, y);
	const uint32_t readMask = 0x3 << indexPosition;
	return (m_indices & readMask) >> indexPosition;
}

inline uint32_t BlockCompressedPixels::GetIndexShiftPosition(uint32_t x, uint32_t y) const
{
	// !Note - y-coordinates are stored in reverse order in DXT1 images, therefore
	// we flip it here transparently (3-y)
	Assert(x < 4 && y < 4);
	return ((3 - y) << 3) + (x << 1);	// ((3 - y) * 8) + (x * 2)
}

BlockCompressedPixels::ColourLUT::ColourLUT(const BlockCompressedPixels& block)
{
	const float c_twoThirds = 2.0f / 3.0f;
	const float c_oneThird = 1.0f / 3.0f;
	const ColourRGB colour0 = block.GetRefColour1();
	const ColourRGB colour1 = block.GetRefColour2();
	const float colour0asFloat[3] = { colour0.GetRedAsFloat(), colour0.GetGreenAsFloat(), colour0.GetBlueAsFloat() };
	const float colour1asFloat[3] = { colour1.GetRedAsFloat(), colour1.GetGreenAsFloat(), colour1.GetBlueAsFloat() };

	m_lut[0] = colour0;
	m_lut[1] = colour1;
	
	//color_2 = 2/3*color_0 + 1/3*color_1
	float tmpColourAsFloat[3];
	tmpColourAsFloat[0] = c_twoThirds * colour0asFloat[0] + c_oneThird * colour1asFloat[0];
	tmpColourAsFloat[1] = c_twoThirds * colour0asFloat[1] + c_oneThird * colour1asFloat[1];
	tmpColourAsFloat[2] = c_twoThirds * colour0asFloat[2] + c_oneThird * colour1asFloat[2];
	m_lut[2] = ColourRGB(tmpColourAsFloat[0], tmpColourAsFloat[1], tmpColourAsFloat[2]);

	//color_3 = 1/3 * color_0 + 2/3 * color_1
	tmpColourAsFloat[0] = c_oneThird * colour0asFloat[0] + c_twoThirds * colour1asFloat[0];
	tmpColourAsFloat[1] = c_oneThird * colour0asFloat[1] + c_twoThirds * colour1asFloat[1];
	tmpColourAsFloat[2] = c_oneThird * colour0asFloat[2] + c_twoThirds * colour1asFloat[2];
	m_lut[3] = ColourRGB(tmpColourAsFloat[0], tmpColourAsFloat[1], tmpColourAsFloat[2]);
}

inline const ColourRGB& BlockCompressedPixels::ColourLUT::GetColour(uint32_t index)
{
	return m_lut[index];
}

#endif