#ifndef IMGCONVERTER_BITMAP_FILE_WRITER_INCLUDED
#define IMGCONVERTER_BITMAP_FILE_WRITER_INCLUDED

class Image;
class RawFileBuffer;
struct BitmapFileHeader;

class BitmapFileWriter
{
public:
	BitmapFileWriter();
	~BitmapFileWriter();

	bool WriteBitmapFile(const Image& sourceData, RawFileBuffer& targetBuffer);

private:
	void GenerateHeader(const Image& sourceData, BitmapFileHeader& header);
};

#endif