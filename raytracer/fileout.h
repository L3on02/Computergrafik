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
        void writeColor(color pixelColor) {
            file << static_cast<int>(255.999 * pixelColor[0]) << " "
                      << static_cast<int>(255.999 * pixelColor[1]) << " "
                      << static_cast<int>(255.999 * pixelColor[2]) << "\n";
        }
        void writeImage(color *image, int width, int height) {
            for (int i = height - 1; i >= 0; i--) {
                for (int j = 0; j < width; j++) {
                    writeColor(image[i * width + j]);
                }
            }
        }
    private:
        std::ofstream file;
};

#endif