#ifndef IMGCONVERTER_COLOUR_24BIT_INCLUDED
#define IMGCONVERTER_COLOUR_24BIT_INCLUDED

class Colour24Bit
{
public:
	inline Colour24Bit();
	inline Colour24Bit(const Colour24Bit& other);
	inline Colour24Bit(Colour24Bit&& other);
	inline explicit Colour24Bit(const uint8_t* b);
	inline explicit Colour24Bit(int32_t r, int32_t g, int32_t b);
	inline explicit Colour24Bit(uint8_t r, uint8_t g, uint8_t b);
	inline explicit Colour24Bit(float r, float g, float b);
	inline ~Colour24Bit();

	inline Colour24Bit& operator=(const Colour24Bit& other);
	inline Colour24Bit& operator=(Colour24Bit&& other);

	inline bool operator==(const Colour24Bit& other) const;
	inline bool operator!=(const Colour24Bit& other) const;

	inline void SetRed(uint8_t r) { m_rgb[0] = r; }
	inline void SetGreen(uint8_t g) { m_rgb[1] = g; }
	inline void SetBlue(uint8_t b) { m_rgb[2] = b; }
	inline uint8_t GetRed() const { return m_rgb[0]; }
	inline uint8_t GetGreen() const { return m_rgb[1]; }
	inline uint8_t GetBlue() const { return m_rgb[2]; }

	inline float GetRedAsFloat() const;
	inline float GetGreenAsFloat() const;
	inline float GetBlueAsFloat() const;

	inline void ToRGBBuffer(uint8_t* targetBuffer) const;

private:
	float ByteToFloatValue(uint8_t v) const;
	uint8_t FloatToByteValue(float v) const;
	uint8_t m_rgb[3];
};

#include "colour_24bit.inl"

#endif