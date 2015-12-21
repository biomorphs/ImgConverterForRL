#ifndef IMGCONVERTER_DXT1_FILE_WRITER_INCLUDED
#define IMGCONVERTER_DXT1_FILE_WRITER_INCLUDED

class BlockCompressedImage;
class RawFileBuffer;
struct DXT1FileHeader;

class DXT1FileWriter
{
public:
	DXT1FileWriter();
	~DXT1FileWriter();

	bool WriteDXT1File(const BlockCompressedImage& sourceImage, RawFileBuffer& targetBuffer);

private:
	void GenerateHeader(const BlockCompressedImage& sourceImage, DXT1FileHeader& header);
};

#endif