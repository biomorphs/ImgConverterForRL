#ifndef IMGCONVERTER_IO_WRAPPERS_INCLUDED
#define IMGCONVERTER_IO_WRAPPERS_INCLUDED

#include <memory>
#include <string>

class Image;
class BlockCompressedImage;

std::unique_ptr<Image> LoadBitmapFromFile(const std::string& path);
bool WriteImageToBitmapFile(const std::unique_ptr<Image>& source, const std::string& destPath);

std::unique_ptr<BlockCompressedImage> LoadDXT1FromFile(const std::string& path);
bool WriteImageToDXT1File(const std::unique_ptr<BlockCompressedImage>& source, const std::string& destPath);

#endif