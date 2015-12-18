#include "catch/catch.hpp"
#include "../ImgLib/raw_file_buffer.h"
#include "../ImgLib/raw_file_io.h"

// Mock objects for faking I/O
class RawFileBufferReaderMock : public RawFileBufferReader
{
public:
	RawFileBufferReaderMock(const uint8_t* fakeData, size_t dataSize)
		: m_fakeData(fakeData), m_fakeDataSize(dataSize) { }
	virtual bool LoadInternal(const std::string& path, RawFileBuffer& target)
	{
		target.AppendBytes(m_fakeData, m_fakeDataSize);
		return true;
	}
private:
	const uint8_t* m_fakeData;
	size_t m_fakeDataSize;
};

class RawFileBufferWriterMock : public RawFileBufferWriter
{
public:
	RawFileBufferWriterMock(std::vector<uint8_t>& outputBuffer)
		: m_outputBuffer(outputBuffer) { }
	virtual bool WriteInternal(const std::string& path, const RawFileBuffer& target)
	{
		m_outputBuffer.clear();
		m_outputBuffer.insert(m_outputBuffer.begin(), target.GetContents(), target.GetContents() + target.GetBufferSize());
		return true;
	}
private:
	std::vector<uint8_t>& m_outputBuffer;
};

TEST_CASE("Empty file name fails to load")
{
	RawFileBufferReader fileReader;
	RawFileBuffer testBuffer;
	REQUIRE(fileReader.LoadFromFile("", testBuffer) == false);
}

TEST_CASE("Empty file results in empty buffer")
{
	RawFileBufferReaderMock reader(nullptr, 0);
	RawFileBuffer testBuffer;
	REQUIRE(reader.LoadFromFile("testFile.test", testBuffer) == true);
	REQUIRE(testBuffer.GetBufferSize() == 0);
}

TEST_CASE("File with data returns correct buffer")
{
	const uint8_t testData[] = { 5, 10, 15, 20, 25, 30 };
	RawFileBufferReaderMock reader(testData, sizeof(testData));
	RawFileBuffer testBuffer;
	REQUIRE(reader.LoadFromFile("testFile.test", testBuffer) == true);
	REQUIRE(testBuffer.GetBufferSize() == sizeof(testData));
	REQUIRE(memcmp(testBuffer.GetContents(), testData, sizeof(testData)) == 0);
}

TEST_CASE("External file reads successfully")
{
	const uint8_t expectedData[] = "12345";
	RawFileBufferReader fileReader;
	RawFileBuffer testBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/testRawFile.txt", testBuffer) == true);
	REQUIRE(testBuffer.GetBufferSize() == 5);
	REQUIRE(memcmp(testBuffer.GetContents(), expectedData, 5) == 0);
}

TEST_CASE("Write with no path fails")
{
	RawFileBufferWriter writer;
	RawFileBuffer testBuffer;
	REQUIRE(writer.WriteTofile("", testBuffer) == false);
}

TEST_CASE("Buffer with data writes correctly")
{
	std::vector<uint8_t> resultbuffer;

	SECTION("Empty buffer writes empty file")
	{
		RawFileBuffer emptyBuffer;
		RawFileBufferWriterMock writer(resultbuffer);
		REQUIRE(writer.WriteTofile("testpath.test", emptyBuffer) == true);
		REQUIRE(resultbuffer.size() == 0);
	}

	SECTION("Buffer data writes successfully")
	{
		const uint8_t testData[] = { 5, 10, 15, 20, 25, 30 };
		RawFileBuffer bufferWithData;
		bufferWithData.AppendBytes(testData, sizeof(testData));

		RawFileBufferWriterMock writer(resultbuffer);
		REQUIRE(writer.WriteTofile("testpath.test", bufferWithData) == true);
		REQUIRE(resultbuffer.size() == sizeof(testData));
		REQUIRE(memcmp(resultbuffer.data(), testData, sizeof(testData)) == 0);;
	}
}

TEST_CASE("Write to external file matches data read")
{
	const uint8_t testData[] = { "some test data" };
	RawFileBuffer sourceBuffer;
	sourceBuffer.AppendBytes(testData, sizeof(testData));

	RawFileBufferWriter fileWriter;
	REQUIRE(fileWriter.WriteTofile("../TestData/testfile.txt", sourceBuffer) == true);

	RawFileBufferReader fileReader;
	RawFileBuffer readBackBuffer;
	REQUIRE(fileReader.LoadFromFile("../TestData/testfile.txt", readBackBuffer) == true);

	REQUIRE(readBackBuffer.GetBufferSize() == sourceBuffer.GetBufferSize());
	REQUIRE(memcmp(readBackBuffer.GetContents(), sourceBuffer.GetContents(), sizeof(testData)) == 0);
}