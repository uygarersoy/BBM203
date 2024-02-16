#ifndef ENCODE_MESSAGE_H
#define ENCODE_MESSAGE_H

#include <string>
#include <vector>
#include "ImageMatrix.h"

class EncodeMessage {
public:
    EncodeMessage();
    ~EncodeMessage();

    ImageMatrix encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions);
    bool isPrime(const int& number);
    int nthFibonacci(const int& n);
    std::string intToBitString(int n);
    std::string reverseString(std::string bitString);

private:
    
};

#endif // ENCODE_MESSAGE_H
