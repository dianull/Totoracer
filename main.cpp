

#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


struct Colour {
    float r;
    float g;
    float b;

    Colour& operator+=(const Colour& rhs) noexcept { 
        r += rhs.r;
        g += rhs.g;
        b += rhs.b;
        return *this; 
    }

    Colour& operator/=(const float rhs) noexcept { 
        r /= rhs;
        g /= rhs;
        b /= rhs;
        return *this; 
    }

    Colour& operator*=(const float rhs) noexcept { 
        r *= rhs;
        g *= rhs;
        b *= rhs;
        return *this; 
    }
};

Colour imageFunc(const double x, const double y, const int x_res, const int y_res) {
    const double centre_x = 300;//x_res / 2;
    const double centre_y = 800;//y_res / 2;

    const double dx = x - centre_x;
    const double dy = y - centre_y;

    const double length_sq = dx * dx + dy * dy;
    const double radius_sq = 640 * 640;

    Colour inside;
    inside.r = 0.7f;
    inside.g = 0.3f;
    inside.b = 0.1f;


    Colour outside;
    outside.r = 0.13f;
    outside.g = 0.01f;
    outside.b = 0.24f;

    return (length_sq <= radius_sq) ? inside : outside;
}

// From https://github.com/skeeto/hash-prospector
constexpr inline uint32_t lowbias32(uint32_t x) {
    x ^= x >> 16; x *= 0x21f0aaad;
    x ^= x >> 15; x *= 0xd35a2d97;
    x ^= x >> 15;
    return x;
}

int main() {

    const int x_res = 1920; 
    const int y_res = 1080;
    const int num_channels = 3; 
    const int num_samples = 16;

    std::vector<uint8_t> image(x_res * y_res * num_channels);
   
    
    for (int y = 0; y < y_res; y++) {
        for (int x = 0; x < x_res; x++) {
            Colour sum;
            sum.r = 0;
            sum.g = 0;
            sum.b = 0;

            for (int z = 0; z < num_samples; z++) {
                const uint32_t x_offset = lowbias32(z * 2 + 0);
                const uint32_t y_offset = lowbias32(z * 2 + 1);
                const double u = x_offset * (1.0 / 4294967296);
                const double v = y_offset * (1.0 / 4294967296);

                Colour c = imageFunc(x + u, y + v, x_res, y_res);
                sum += c;
            }
            sum /= num_samples;
            sum *= 255.0f;

            const int pixel_idx = y * x_res + x;
            image[pixel_idx * 3 + 0] = sum.r;
            image[pixel_idx * 3 + 1] = sum.g;
            image[pixel_idx * 3 + 2] = sum.b;
          
        }
    }
    stbi_write_png("hallo.png", x_res, y_res, num_channels, image.data(), x_res * num_channels);
}



