#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {
    Gx = new double*[3];
    Gy = new double*[3];

    for (int i = 0; i < 3; i++) {
        Gx[i] = new double[3];
        Gy[i] = new double[3];
    }

    Gx[0][0] = -1; Gx[0][1] = 0; Gx[0][2] = 1;
    Gx[1][0] = -2; Gx[1][1] = 0; Gx[1][2] = 2;
    Gx[2][0] = -1; Gx[2][1] = 0; Gx[2][2] = 1;

    Gy[0][0] = -1; Gy[0][1] = -2; Gy[0][2] = -1;
    Gy[1][0] = 0;  Gy[1][1] = 0;  Gy[1][2] = 0;
    Gy[2][0] = 1;  Gy[2][1] = 2;  Gy[2][2] = 1;
    
}

// Destructor
EdgeDetector::~EdgeDetector() {
    if (Gx != nullptr) {
        for (int i = 0; i < 3; i++) {
            delete[] Gx[i];
        }
        delete[] Gx;
    }
    
    if (Gy != nullptr) {
        for (int i = 0; i < 3; i++) {
            delete[] Gy[i];
        }
        delete[] Gy;
    }
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    std::vector<std::pair<int, int>> edges;
    double sum = 0.0;

    Convolution gx_convolve(Gx, 3, 3, 1, true);
    Convolution gy_convolve(Gy, 3, 3, 1, true);

    ImageMatrix Ix = gx_convolve.convolve(input_image);
    ImageMatrix Iy = gy_convolve.convolve(input_image);

    ImageMatrix temp(input_image);
    int height = temp.get_height();
    int width = temp.get_width();

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            double new_value = std::sqrt(std::pow(Ix.get_data(i, j), 2) + std::pow(Iy.get_data(i, j), 2));
            temp.set_data(new_value, i, j);
            sum += new_value;
        }
    }

    double threshold = sum / (height * width);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (temp.get_data(i, j) > threshold) {
                edges.push_back({i, j});
            }
        }
    }

    return edges;       
}