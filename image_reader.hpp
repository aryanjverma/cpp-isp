#ifndef IMAGE_READER_HPP
#define IMAGE_READER_HPP
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;
vector<float> read_image(string path);
/*
int main(){
    vector<float> image = read_image("dataset/8/8/0.png");
    return 0;
}
*/
vector<float> read_image(string path){
    int width, height, channels;
    const char* c = path.c_str();
    unsigned char* img = stbi_load(c, &width, &height, &channels, 0);
    vector<float> image;
    if (!img) {
        cerr << "Failed to load image!\n";
        return image;
    }

    cout << "Image Loaded: " << width << "x" << height << " with " << channels << " channels\n";

    // Loop through each pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;  // Pixel index
            bool is_transparent = channels == 4 && img[index + 3] == 0;  // Check if pixel is transparent
            if (is_transparent) {
                
                image.push_back(0);
            } else {
                
                image.push_back(1);
            }
            
        }
        
    }

    stbi_image_free(img);  // Free memory
    return image;
}

#endif