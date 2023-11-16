#ifndef PARALLEL_H
#define PARALLEL_H

#include "geometry.h"

#include <thread>
#include <vector>

#include <mutex>

using color = vec3;

class image_memory {
    public:
        image_memory(int render_width, int render_height) : lines(render_height), rows(render_width) {
            linesLeft = lines;
            shared_storage = new color[lines * rows];
        }

        void write_pixel(int line, int row, color pixel_color) {
            std::lock_guard<std::mutex> guard(lock_img);
            shared_storage[(line - 1) * rows + row] = pixel_color;
        }

        int get_render_line() {
            std::lock_guard<std::mutex> guard(lock_line);
            std::clog << "\rLines remaining: " << fmax(linesLeft, 0) << std::flush;
            return (lines - (--linesLeft));
        }

        color* get_image() {
            std::lock_guard<std::mutex> guard(lock_img);
            return shared_storage;
        }

    private:
        int lines;
        int rows;
        std::mutex lock_img;
        color* shared_storage;

        int linesLeft;
        std::mutex lock_line;
};

#endif