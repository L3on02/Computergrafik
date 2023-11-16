#ifndef FILEOUT_H
#define FILEOUT_H

#include <fstream>
#include "geometry.h"

class fileout {
    public:
        fileout(int width, int height) {
            file.open("image.ppm");
            file << "P3\n" << width << " " << height << "\n255\n";
        }
        ~fileout() {
            file.close();
        }
        void writePixel(color pixelColor) {
            file << static_cast<int>(255.999 * pixelColor[0]) << " "
                      << static_cast<int>(255.999 * pixelColor[1]) << " "
                      << static_cast<int>(255.999 * pixelColor[2]) << "\n";
        }
        void writeImage(color *image, int width, int height) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    writePixel(image[i * width + j]);
                }
            }
        }
    private:
        std::ofstream file;
};

#endif