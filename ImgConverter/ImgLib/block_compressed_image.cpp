#include "block_compressed_image.h"

BlockCompressedImage::BlockCompressedImage(uint32_t widthPixels, uint32_t heightPixels)
	: m_widthPixels(0)
	, m_heightPixels(0)
	, m_widthBlocks(0)
	, m_heightBlocks(0)
{
	// Block-compressed images must have size a multiple of 4 pixels
	if ((widthPixels & 3) == 0 && (heightPixels & 3) == 0 && widthPixels >= 4 && heightPixels >= 4)
	{
		m_widthPixels = widthPixels;
		m_heightPixels = heightPixels;
		m_widthBlocks = widthPixels / 4;
		m_heightBlocks = heightPixels / 4;
		m_pixelData.resize(m_widthBlocks * m_heightBlocks);
	}
}

BlockCompressedImage::BlockCompressedImage(BlockCompressedImage&& other)
{
	*this = std::move(other);
}

BlockCompressedImage::~BlockCompressedImage()
{
}

BlockCompressedImage& BlockCompressedImage::operator=(BlockCompressedImage&& other)
{
	m_pixelData = std::move(other.m_pixelData);
	m_widthPixels = other.m_widthPixels;
	m_heightPixels = other.m_heightPixels;
	m_widthBlocks = other.m_widthBlocks;
	m_heightBlocks = other.m_heightBlocks;
	other.m_widthPixels = 0;
	other.m_heightPixels = 0;
	other.m_widthBlocks = 0;
	other.m_heightBlocks = 0;
	return *this;
}

void BlockCompressedImage::GetPixelColour(uint32_t x, uint32_t y, ColourRGB& colour) const
{
	// !Note - y-axis is flipped in DXT1-compressed images
	if (x < m_widthPixels && y < m_heightPixels)
	{
		const uint32_t flippedY = m_heightPixels - 1 - y;
		const uint32_t blockX = x >> 2;				// x / 4
		const uint32_t blockY = flippedY >> 2;		// y / 4
		const auto& sourceBlock = GetBlock(blockX, blockY);
		BlockCompressedPixels::ColourLUT lut(sourceBlock);
		const uint32_t pixelColourIndex = sourceBlock.GetPixelColourIndex(x & 3, flippedY & 3);	// extract pixel in block
		colour = lut.GetColour(pixelColourIndex);
	}
}

BlockCompressedPixels& BlockCompressedImage::GetBlock(uint32_t blckX, uint32_t blckY)
{
	return m_pixelData[(blckY * m_widthBlocks) + blckX];
}

const BlockCompressedPixels& BlockCompressedImage::GetBlock(uint32_t blckX, uint32_t blckY) const
{
	return m_pixelData[(blckY * m_widthBlocks) + blckX];
}

BlockCompressedPixels* BlockCompressedImage::BlockAt(uint32_t x, uint32_t y)
{
	// !Note - y-axis is flipped in DXT1-compressed images
	if (x < m_widthBlocks && y < m_heightBlocks)
	{
		return &GetBlock(x, m_heightBlocks - y - 1);
	}
	return nullptr;
}

const BlockCompressedPixels* BlockCompressedImage::BlockAt(uint32_t x, uint32_t y) const
{
	// !Note - y-axis is flipped in DXT1-compressed images
	if (x < m_widthBlocks && y < m_heightBlocks)
	{
		return &GetBlock(x, m_heightBlocks - y - 1);
	}
	return nullptr;
}