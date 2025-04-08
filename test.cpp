#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

int main() {
    int width, height, channels;
    unsigned char* img = stbi_load("image.png", &width, &height, &channels, 0);

    if (!img) {
        std::cerr << "Failed to load image!\n";
        return 1;
    }

    std::cout << "Image Loaded: " << width << "x" << height 
              << " with " << channels << " channels\n";

    // Loop through each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;  // Pixel index
            
            unsigned char r = img[index];   // Red
            unsigned char g = img[index + 1]; // Green
            unsigned char b = img[index + 2]; // Blue
            
            // Check if the image has an alpha (transparency) channel
            bool is_transparent = (channels == 4) && (img[index + 3] == 0);

            // Check if the pixel is black (R = G = B = 0) and **not transparent**
            bool is_black = (!is_transparent && r == 0 && g == 0 && b == 0);

            // Print 1 for black, 0 for non-black (including transparent pixels)
            std::cout << (is_black ? "1 " : "0 ");
        }
        std::cout << "\n";  // New line after each row
    }

    stbi_image_free(img);  // Free memory
    return 0;
}
