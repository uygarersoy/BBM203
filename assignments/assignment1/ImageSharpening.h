#ifndef IMAGE_SHARPENING_H
#define IMAGE_SHARPENING_H

#include "ImageMatrix.h"
#include "Convolution.h"

class ImageSharpening {
    public:
        ImageSharpening(); // default constructor
        ~ImageSharpening();  // default destructor

        ImageMatrix sharpen(const ImageMatrix& input_image, double k);  
        double** get_sharpening_kernel();
    private:
        double** blurring_kernel;
};

#endif // IMAGE_SHARPENING_H



