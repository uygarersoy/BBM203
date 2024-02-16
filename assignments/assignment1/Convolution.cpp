#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() : kernelHeight(1), kernelWidth(1), stride(1), padding(false), kernel(nullptr) {
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) : kernelHeight(kh), kernelWidth(kw), stride(stride_val), padding(pad) {
    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; i++) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; i++) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other): kernelHeight(other.kernelHeight), kernelWidth(other.kernelWidth), stride(other.stride), padding(other.padding) {
    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; i++) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = other.get_kernel_value(i, j);
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this;
    }

    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; i++) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;

    kernel = new double*[kernelHeight];

    for (int i = 0; i < kernelHeight; i++) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = other.get_kernel_value(i, j);
        }
    }
    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    ImageMatrix image(input_image);
    int image_height = image.get_height();
    int image_width = image.get_width();
    if (padding) {
        ImageMatrix padded_image(input_image.get_height() + 2, input_image.get_width() + 2);
        for (int i = 1; i < padded_image.get_height() - 1; i++) {
            for (int j = 1; j < padded_image.get_width() - 1; j++) {
                padded_image.set_data(image.get_data(i-1,j-1), i, j);
            }
        }

        image = padded_image;
    }
    
    int height = (((image_height - get_kernel_height()) + 2 * padding) / stride) + 1;
    int width = (((image_width - get_kernel_width()) + 2 * padding) / stride) + 1;

    ImageMatrix convolved_image(height, width);

    for (int imageRow = 0; imageRow < height; imageRow++) {
        for (int imageColumn = 0; imageColumn < width; imageColumn++) {
            double pixel_value = 0;
            for (int kernelRow = 0; kernelRow < kernelHeight; kernelRow++) {
                for (int kernelColumn = 0; kernelColumn < kernelWidth; kernelColumn++) {
                    pixel_value += image.get_data(imageRow * stride + kernelRow, imageColumn * stride + kernelColumn) * kernel[kernelRow][kernelColumn];
                }
            }
            convolved_image.set_data(pixel_value, imageRow, imageColumn);
        }
    }

    return convolved_image;
}

int Convolution::get_kernel_height() const {
    return kernelHeight;
}

int Convolution::get_kernel_width() const {
    return kernelWidth;
}

double Convolution::get_kernel_value(int i, int j) const {
    return kernel[i][j];
}