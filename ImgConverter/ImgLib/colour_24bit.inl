#ifndef IMGCONVERTER_COLOUR_24BIT_INL_INCLUDED
#define IMGCONVERTER_COLOUR_24BIT_INL_INCLUDED

#include "utils.h"

Colour24Bit::Colour24Bit()
{
}

Colour24Bit::Colour24Bit(const Colour24Bit& other)
{
	m_rgb[0] = other.m_rgb[0];
	m_rgb[1] = other.m_rgb[1];
	m_rgb[2] = other.m_rgb[2];
}

Colour24Bit::Colour24Bit(Colour24Bit&& other)
{
	m_rgb[0] = other.m_rgb[0];
	m_rgb[1] = other.m_rgb[1];
	m_rgb[2] = other.m_rgb[2];
}

Colour24Bit::Colour24Bit(const uint8_t* b)
{
	Assert(b != nullptr);
	m_rgb[0] = b[0];
	m_rgb[1] = b[1];
	m_rgb[2] = b[2];
}

Colour24Bit::Colour24Bit(int32_t r, int32_t g, int32_t b)
{
	m_rgb[0] = static_cast<uint8_t>( r );
	m_rgb[1] = static_cast<uint8_t>( g );
	m_rgb[2] = static_cast<uint8_t>( b );
}

Colour24Bit::Colour24Bit(uint8_t r, uint8_t g, uint8_t b)
{
	m_rgb[0] = r;
	m_rgb[1] = g;
	m_rgb[2] = b;
}

Colour24Bit::Colour24Bit(float r, float g, float b)
{
	m_rgb[0] = FloatToByteValue(r);
	m_rgb[1] = FloatToByteValue(g);
	m_rgb[2] = FloatToByteValue(b);
}

Colour24Bit::~Colour24Bit()
{

}

Colour24Bit& Colour24Bit::operator=(const Colour24Bit& other)
{
	m_rgb[0] = other.m_rgb[0];
	m_rgb[1] = other.m_rgb[1];
	m_rgb[2] = other.m_rgb[2];
}

Colour24Bit& Colour24Bit::operator=(Colour24Bit&& other)
{
	m_rgb[0] = other.m_rgb[0];
	m_rgb[1] = other.m_rgb[1];
	m_rgb[2] = other.m_rgb[2];
}

bool Colour24Bit::operator==(const Colour24Bit& other) const
{
	return (m_rgb[0] == other.m_rgb[0] && m_rgb[1] == other.m_rgb[1] && m_rgb[2] == other.m_rgb[2]);
	return false;
}

bool Colour24Bit::operator!=(const Colour24Bit& other) const
{
	return !(other == *this);
}

float Colour24Bit::GetRedAsFloat() const
{
	return ByteToFloatValue( m_rgb[0] );
}

float Colour24Bit::GetGreenAsFloat() const
{
	return ByteToFloatValue(m_rgb[1]);
}

float Colour24Bit::GetBlueAsFloat() const
{
	return ByteToFloatValue(m_rgb[2]);
}

void Colour24Bit::ToRGBBuffer(uint8_t* targetBuffer) const
{
	Assert(targetBuffer != nullptr);
	targetBuffer[0] = m_rgb[0];
	targetBuffer[1] = m_rgb[1];
	targetBuffer[2] = m_rgb[2];
}

float Colour24Bit::ByteToFloatValue(uint8_t v) const
{
	float byteAsFloatRaw = static_cast<float>(v);
	return byteAsFloatRaw * (1.0f / 255.0f);
}

uint8_t Colour24Bit::FloatToByteValue(float v) const
{
	float quantisedFloat = v * 255.0f;
	return static_cast<uint8_t>(quantisedFloat);
}

#endif