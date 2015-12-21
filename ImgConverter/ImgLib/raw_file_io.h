#ifndef IMGCONVERTER_RAW_FILE_IO_INCLUDED
#define IMGCONVERTER_RAW_FILE_IO_INCLUDED

#include "utils.h"
#include <string>

class RawFileBuffer;

class RawFileBufferReader
{
public:
	RawFileBufferReader();
	~RawFileBufferReader();
	bool LoadFromFile(const std::string& path, RawFileBuffer& target);

private:
	MOCKABLE bool LoadInternal(const std::string& path, RawFileBuffer& target);
};

class RawFileBufferWriter
{
public:
	RawFileBufferWriter();
	~RawFileBufferWriter();
	bool WriteTofile(const std::string& path, const RawFileBuffer& sourceData);

private:
	MOCKABLE bool WriteInternal(const std::string& path, const RawFileBuffer& sourceData);
};

#endif