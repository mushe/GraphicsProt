#include "File.h"
#include <iostream>
namespace File
{
    std::map < std::string, std::vector<char> > shaderFileCache_;
    std::map < std::string, ImageFile* > imageFileCache_;
}

std::vector<char> File::ReadShaderFile(const std::string& filename)
{
    if (shaderFileCache_[filename].size() > 0)
    {
        return shaderFileCache_[filename];
    }

    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    assert(file.is_open());

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    shaderFileCache_[filename] = buffer;
    return buffer;
}

File::ImageFile* File::ReadImageFile(const std::string filename)
{
    if (imageFileCache_.count(filename) >0)
    {
        return imageFileCache_[filename];
    }

    int width, height, channel;
    auto textureImage = stbi_load(filename.c_str(), &width, &height, &channel, STBI_rgb_alpha);
    imageFileCache_[filename] = new ImageFile{textureImage, width, height, channel, STBI_rgb_alpha };
    return imageFileCache_[filename];
}

void File::ReleaseImageCache()
{
	for (auto& image : imageFileCache_)
	{
		stbi_image_free(image.second->data);
		delete image.second;
	}
	imageFileCache_.clear();
}
