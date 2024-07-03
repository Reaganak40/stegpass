#include "pch.h"
#include "Image.hpp"
#include "Logging.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize2.h"

bool sp::Image::LoadTextureFromFile(const char* filename)
{
    // check if the file exists
    if (!std::filesystem::exists(filename)) {
        SP_LOG_WARN("Could not find file: {}", std::string(filename));
        return false;
    }


    int channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);

    if (!data) {
        SP_LOG_ERROR("Failed to load image: {}", std::string(filename));
        return false;
    }

    // Get the maximum supported texture size
    GLint max_texture_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_texture_size);

    bool is_aligned = (width % 4 == 0) && (height % 4 == 0);
    bool is_too_large = width > max_texture_size || height > max_texture_size;

    if (!is_aligned || is_too_large) {

        int new_width = width;
        int new_height = height;

        // get new width and height that maintains aspect ratio and is under the max texture size
        if (is_too_large) {
            float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);
            int new_width = std::min(width, max_texture_size);
            int new_height = static_cast<int>(new_width / aspect_ratio);

            if (new_height > max_texture_size) {
                new_height = max_texture_size;
                new_width = static_cast<int>(new_height * aspect_ratio);
            }
        }

        if (new_width % 4 != 0) {
			new_width += 4 - (new_width % 4);
		}

        if (new_height % 4 != 0) {
            new_height += 4 - (new_height % 4);
        }
        unsigned char* resized_data = new unsigned char[new_width * new_height * channels];
        stbir_resize_uint8_linear(data, width, height, 0, resized_data, new_width, new_height, 0, (stbir_pixel_layout)channels);

        stbi_image_free(data);
        data = resized_data;
        width = new_width;
        height = new_height;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, (channels == 4 ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    SP_LOG_TRACE("Loaded image: {}", std::string(filename));
    return textureID;
}

ImVec2 sp::Image::GetSizeWithMaintainedAspectRatio(float maxWidth, float maxHeight) const
{
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	float newWidth = maxWidth;
	float newHeight = newWidth / aspectRatio;

	if (newHeight > maxHeight) {
		newHeight = maxHeight;
		newWidth = newHeight * aspectRatio;
	}

	return ImVec2(newWidth, newHeight);
}

sp::ImageFormat sp::GetImageFormat(const std::string& filename)
{
    // get the extension of the file
    size_t last_dot = filename.find_last_of('.');
    if (last_dot == std::string::npos) {
		return ImageFormat::SP_IMAGE_FORMAT_UNSUPPORTED;
	}
    std::string extension = filename.substr(last_dot + 1);

    // convert the extension to lowercase
    std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char c) { return std::tolower(c); });

    // check if the extension is supported
    if (strcmp("bmp", extension.c_str()) == 0) {
        return ImageFormat::SP_IMAGE_FORMAT_BMP;
    }

    return ImageFormat::SP_IMAGE_FORMAT_UNSUPPORTED;
}
