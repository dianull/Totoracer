

#include <iostream>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "maths.h"

struct World {
    std::vector<Circle> circles;

};

Colour imageFunc(const World & world, const double x, const double y, const int x_res, const int y_res) {

    Colour inside;
    inside.r = 0.7f;
    inside.g = 0.3f;
    inside.b = 0.1f;

    Colour outside;
    outside.r = 0.13f;
    outside.g = 0.01f;

    outside.b = 0.24f;
    
    Colour res = outside;

    for (int i = 0; i < world.circles.size(); i++) {
        const double centre_x = world.circles[i].centre.x;
        const double centre_y = world.circles[i].centre.y;

        const double dx = x - centre_x; 
        const double dy = y - centre_y;

        const double length_sq = dx * dx + dy * dy;
        const double radius_sq = world.circles[i].radius * world.circles[i].radius;
        if (length_sq <= radius_sq) {
            res = inside;
            break;
        }
    }

    return res;
}

Circle getCircle(int x_res, int y_res) {
        const int radius_max = 100;
        const int radius_min = 20;

        double centre_x = std::rand() % (x_res - radius_max + 1) + radius_max;
       
        const double centre_y =  std::rand() % (y_res - radius_max + 1) + radius_max;

        Colour colour;
        colour.r = 0.7f;
        colour.g = 0.3f;
        colour.b = 0.1f;

        Circle circle;
        Vec2 v;
        v.x = centre_x ;
        v.y = centre_y;
        circle.centre = v;
        circle.colour = colour;
        circle.radius = std::rand() % (radius_max - radius_min + 1) + radius_min;

        return circle;
}

int main() {

    const int x_res = 1920; 
    const int y_res = 1080;
    
    const int num_channels = 3; 
    const int num_samples = 16;
    const int num_circles = 5;

    std::vector<uint8_t> image(x_res * y_res * num_channels);

    std::vector<Circle> circles(num_circles);
    World world;
    world.circles = circles;
    
    for (int i = 0; i < num_circles; i++) {         
        Circle circle = getCircle(x_res, y_res);
        circles[i] = circle;
    }

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

             //   Circle c;
             //   c.centre.x = u * x_res;
             //   c.centre.y = v * y_res;
              //  world.circles.push_back(c);
                Colour c = imageFunc(world, x + u, y + v, x_res, y_res);
                sum += c;
            }
        //    Colour c = imageFunc(world, x, y, x_res, y_res);
         //   sum += c;
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



