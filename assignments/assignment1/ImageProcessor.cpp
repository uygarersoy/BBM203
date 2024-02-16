#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {
    k_factor = 2.0;
}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageSharpening sharpener;
    ImageMatrix sharpened = sharpener.sharpen(img, k_factor);
    EdgeDetector edge_detect;
    std::vector<std::pair<int, int>> edges = edge_detect.detectEdges(sharpened);
    DecodeMessage decode;
    std::string message = decode.decodeFromImage(sharpened, edges);

    return message;

}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageSharpening sharpener;
    ImageMatrix sharpened = sharpener.sharpen(img, k_factor);
    EdgeDetector edge_detect;
    std::vector<std::pair<int, int>> edges = edge_detect.detectEdges(sharpened);
    EncodeMessage encoder;
    ImageMatrix encoded_image = encoder.encodeMessageToImage(img, message, edges);
    return encoded_image;
}
