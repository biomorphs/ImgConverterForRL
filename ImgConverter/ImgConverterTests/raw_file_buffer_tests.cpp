#include "catch/catch.hpp"
#include "../ImgLib/raw_file_buffer.h"
#include "../ImgLib/raw_file_io.h"

TEST_CASE("RawFileBuffer with size initialised correctly")
{
	RawFileBuffer fileBufferWithSize(1024);
	REQUIRE(fileBufferWithSize.GetBufferSize() == 1024);
	REQUIRE(fileBufferWithSize.GetContents() != nullptr);
}

TEST_CASE("Append data to RawFileBuffer")
{
	RawFileBuffer testBuffer;
	const uint8_t testData[] = { 0, 2, 4, 6, 8, 10, 12, 16 };
	testBuffer.AppendBytes(testData, sizeof(testData));

	SECTION("Single append is ok")
	{
		REQUIRE(testBuffer.GetBufferSize() == sizeof(testData));
		REQUIRE(testBuffer.GetContents() != nullptr);
		REQUIRE(memcmp(testBuffer.GetContents(), testData, sizeof(testData)) == 0);
	}

	SECTION("Multiple appends dont overwrite")
	{
		const uint8_t moreTestData[] = { 18, 20, 22, 24, 26, 28, 30, 32 };
		testBuffer.AppendBytes(moreTestData, sizeof(moreTestData));
		REQUIRE(testBuffer.GetBufferSize() == (sizeof(testData) + sizeof(moreTestData)));
		REQUIRE(memcmp(testBuffer.GetContents(), testData, sizeof(testData)) == 0);
		REQUIRE(memcmp(testBuffer.GetContents() + sizeof(testData), moreTestData, sizeof(moreTestData)) == 0);
	}
}