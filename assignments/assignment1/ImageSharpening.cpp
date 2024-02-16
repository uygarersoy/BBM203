#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    blurring_kernel = new double*[3];

    for (int i = 0; i < 3; i++) {
        blurring_kernel[i] = new double[3];
        for (int j = 0; j < 3; j++) {
            blurring_kernel[i][j] = 1 / 9.0;
        }
    }
}

ImageSharpening::~ImageSharpening(){
    if (blurring_kernel != nullptr) {
        for (int i = 0; i < 3; i++) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
    }
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    Convolution blurring_convol(blurring_kernel, 3, 3, 1, true);
    ImageMatrix blurred_image = blurring_convol.convolve(input_image);
    ImageMatrix sharp_image = input_image + ((input_image - blurred_image) * k);
    int height = sharp_image.get_height();
    int width = sharp_image.get_width();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (sharp_image.get_data(i, j) > 255) {
                sharp_image.set_data(255.0, i, j);
            }
            else if (sharp_image.get_data(i, j) < 0) {
                sharp_image.set_data(0.0, i, j);
            }
        }
    }

    return sharp_image;    
}

double** ImageSharpening::get_sharpening_kernel() {
    return blurring_kernel;
}