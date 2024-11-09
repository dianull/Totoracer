

#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "maths.h"

struct World {
    std::vector<Circle> circles;
};

// From https://github.com/skeeto/hash-prospector
constexpr inline uint32_t lowbias32(uint32_t x) {
	x ^= x >> 16; x *= 0x21f0aaad;
	x ^= x >> 15; x *= 0xd35a2d97;
	x ^= x >> 15;
	return x;
}

float sRGB(float val) { 
	if (val < 0.0031308f)
		val *= 12.92f;
	else
		val = 1.055f * powf(val, 1.0f / 2.4f) - 0.055f;
	return val;
}

Colour imageFunc(const World & world, const double x, const double y, const int x_res, const int y_res) {

    Colour inside;
    inside.r = 0.7f;
    inside.g = 0.3f;
    inside.b = 0.1f;

    Colour outside;
    outside.r = 0.13f;
    outside.g = 0.01f;
    outside.b = 0.24f;
    
    for (int i = 0; i < world.circles.size(); i++) {
        const double centre_x = world.circles[i].centre.x;
        const double centre_y = world.circles[i].centre.y;

        const double dx = x - centre_x; 
        const double dy = y - centre_y;

        const double length_sq = dx * dx + dy * dy;
        const double radius_sq = world.circles[i].radius * world.circles[i].radius;
        if (length_sq <= radius_sq) {
            return inside;
        }
    }

    return outside;
}

int main() {

    const int x_res = 1920; 
    const int y_res = 1080;
    
    const int num_channels = 3; 
    const int num_samples = 16;
    const int num_circles = 5;

    std::vector<uint8_t> image(x_res * y_res * num_channels);
  
    World world;

	for (int z = 0; z < num_samples; z++) {
		const uint32_t x_offset = lowbias32(z * 2 + 0);
		const uint32_t y_offset = lowbias32(z * 2 + 1);
		const double u = x_offset * (1.0 / 4294967296);
		const double v = y_offset * (1.0 / 4294967296);

		Circle circle;
		circle.centre.x = u * x_res;
		circle.centre.y = v * y_res;
		circle.colour.r = 0.7f;
		circle.colour.g = 0.3f;
		circle.colour.b = 0.1f;
		circle.radius = 16;// std::rand() % (200 - 100 + 1) + 20;
		world.circles.push_back(circle);
	}

    #pragma omp parallel for
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

                //const double nonuniform_but_still_random = u * u; // output range is still [0,1)

                Colour c = imageFunc(world, x + u, y + v, x_res, y_res);
                sum += c;
		    }
            
            Colour avg_linear;
            avg_linear.r = sum.r / num_samples;
            avg_linear.g = sum.g / num_samples;
            avg_linear.b = sum.b / num_samples;

            Colour avg_sRGB;
            avg_sRGB.r = sRGB(avg_linear.r);
            avg_sRGB.g = sRGB(avg_linear.g);
            avg_sRGB.b = sRGB(avg_linear.b);

            avg_sRGB *= 255.0f;

            const int pixel_idx = y * x_res + x;
            image[pixel_idx * 3 + 0] = int(avg_sRGB.r);
            image[pixel_idx * 3 + 1] = int(avg_sRGB.g);
            image[pixel_idx * 3 + 2] = int(avg_sRGB.b);
        }
    }    
     
    stbi_write_png("hallo.png", x_res, y_res, num_channels, image.data(), x_res * num_channels);
}



