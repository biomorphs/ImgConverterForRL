#ifndef IMGCONVERTER_COLOUR_RGB_INL_INCLUDED
#define IMGCONVERTER_COLOUR_RGB_INL_INCLUDED

#include "utils.h"

ColourRGB::ColourRGB()
{
}

ColourRGB::ColourRGB(const ColourRGB& other)
{
	m_rgb[0] = other.m_rgb[0];
	m_rgb[1] = other.m_rgb[1];
	m_rgb[2] = other.m_rgb[2];
}

ColourRGB::ColourRGB(ColourRGB&& other)
{
	m_rgb[0] = other.m_rgb[0];
	m_rgb[1] = other.m_rgb[1];
	m_rgb[2] = other.m_rgb[2];
}

ColourRGB::ColourRGB(const uint8_t* b)
{
	Assert(b != nullptr);
	m_rgb[0] = b[0];
	m_rgb[1] = b[1];
	m_rgb[2] = b[2];
}

ColourRGB::ColourRGB(int32_t r, int32_t g, int32_t b)
{
	m_rgb[0] = static_cast<uint8_t>( r );
	m_rgb[1] = static_cast<uint8_t>( g );
	m_rgb[2] = static_cast<uint8_t>( b );
}

ColourRGB::ColourRGB(uint8_t r, uint8_t g, uint8_t b)
{
	m_rgb[0] = r;
	m_rgb[1] = g;
	m_rgb[2] = b;
}

ColourRGB::ColourRGB(float r, float g, float b)
{
	m_rgb[0] = FloatToByteValue(r);
	m_rgb[1] = FloatToByteValue(g);
	m_rgb[2] = FloatToByteValue(b);
}

ColourRGB::~ColourRGB()
{

}

ColourRGB& ColourRGB::operator=(const ColourRGB& other)
{
	m_rgb[0] = other.m_rgb[0];
	m_rgb[1] = other.m_rgb[1];
	m_rgb[2] = other.m_rgb[2];
	return *this;
}

ColourRGB& ColourRGB::operator=(ColourRGB&& other)
{
	m_rgb[0] = other.m_rgb[0];
	m_rgb[1] = other.m_rgb[1];
	m_rgb[2] = other.m_rgb[2];
}

bool ColourRGB::operator==(const ColourRGB& other) const
{
	return (m_rgb[0] == other.m_rgb[0] && m_rgb[1] == other.m_rgb[1] && m_rgb[2] == other.m_rgb[2]);
	return false;
}

bool ColourRGB::operator!=(const ColourRGB& other) const
{
	return !(other == *this);
}

float ColourRGB::GetRedAsFloat() const
{
	return ByteToFloatValue( m_rgb[0] );
}

float ColourRGB::GetGreenAsFloat() const
{
	return ByteToFloatValue(m_rgb[1]);
}

float ColourRGB::GetBlueAsFloat() const
{
	return ByteToFloatValue(m_rgb[2]);
}

void ColourRGB::ToRGBBuffer(uint8_t* targetBuffer) const
{
	Assert(targetBuffer != nullptr);
	targetBuffer[0] = m_rgb[0];
	targetBuffer[1] = m_rgb[1];
	targetBuffer[2] = m_rgb[2];
}

float ColourRGB::ByteToFloatValue(uint8_t v) const
{
	float byteAsFloatRaw = static_cast<float>(v);
	return byteAsFloatRaw * (1.0f / 255.0f);
}

uint8_t ColourRGB::FloatToByteValue(float v) const
{
	float quantisedFloat = v * 255.0f;
	return static_cast<uint8_t>(quantisedFloat);
}

#endif