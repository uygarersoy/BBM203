#include "DecodeMessage.h"
#include <cmath>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    lsb = "";
    message = "";

    for (std::pair<int, int> p : edgePixels) {
        int odd_even = static_cast<int>(image.get_data(p.first, p.second)) % 2;
        lsb += std::to_string(odd_even);
    }

    if (lsb.size() % 7) {
        int length = 7 - (lsb.size() % 7);
        for (int i = 0; i < length; i++) {
            lsb = "0" + lsb;
        }
    }

    for (int i = 0; i < lsb.size(); i += 7) {
        std::string temp = "";
        for (int j = i; j < i + 7; j++) {
            temp += lsb[j];
        }
        int decimal = bitStringToInteger(temp);
        if (decimal < 33) {
            decimal += 33;
        }
        else if (decimal == 127) {
            decimal = 126;
        }
        message += static_cast<char>(decimal);
    }
    return message;

}

int DecodeMessage::bitStringToInteger(const std::string& bitString) {
    int total = 0;
    int length = bitString.size();
    for (int i = 0; i < length; i++) {
        total += (std::pow(2,length - i - 1) * (bitString[i] == '1'));
    }
    return total;
}
