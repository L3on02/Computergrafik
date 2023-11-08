#ifndef COLOR_H
#define COLOR_H

#include <fstream>
#include "geometry.h"

class fileOut {
    public:
        fileOut(int width, int height) {
            file << "P3\n" << width << " " << height << "\n255\n";
        }
        ~fileOut() {
            file.close();
        }
        void writeColor(color pixelColor) {
            file << static_cast<int>(255.999 * pixelColor[0]) << " "
                      << static_cast<int>(255.999 * pixelColor[1]) << " "
                      << static_cast<int>(255.999 * pixelColor[2]) << "\n";
        }
    private:
        std::ofstream file;
};

#endif