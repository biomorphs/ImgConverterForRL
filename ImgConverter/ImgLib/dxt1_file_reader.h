#ifndef IMGCONVERTER_DXT1_FILE_READER_INCLUDED
#define IMGCONVERTER_DXT1_FILE_READER_INCLUDED

#include <memory>

class RawFileBuffer;
class BlockCompressedImage;
struct DXT1FileHeader;

class DXT1FileReader
{
public:
	DXT1FileReader();
	~DXT1FileReader();

	bool ExtractHeader(const RawFileBuffer& data, DXT1FileHeader& targetHeader);
	bool IsFormatSupported(const DXT1FileHeader& header);
	std::unique_ptr<BlockCompressedImage> ExtractImage(const RawFileBuffer& data);
};

#endif