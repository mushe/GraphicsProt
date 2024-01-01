#pragma once
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "stb_image.h"

namespace File
{
	struct ImageFile
	{
		stbi_uc* data;
		int width;
		int height;
		int channel;
		int req_comp;
	};

    std::vector<char> ReadShaderFile(const std::string& filename);

	ImageFile* ReadImageFile(const std::string filename);

	void ReleaseImageCache();
}