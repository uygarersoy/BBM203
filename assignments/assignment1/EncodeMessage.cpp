#include "EncodeMessage.h"
#include <cmath>

// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    
    std::string updated_message = "";
    ImageMatrix encoded_image(img);

    for (int i = 0; i < message.size(); i++) {
        if (isPrime(i)) {
            int nth_fib = nthFibonacci(i);
            int ascii_value = static_cast<int>(message[i]);
            ascii_value += nth_fib;
            if (ascii_value < 33) {
                ascii_value += 33;
            }
            else if (ascii_value > 126) {
                ascii_value = 126;
            }
            updated_message += static_cast<char>(ascii_value);
        }
        else {
            updated_message += message[i];
        }
    }
    std::string shifted_message = "";
    int shift_num = updated_message.size() / 2;

    for (int i = updated_message.size() - shift_num; i < updated_message.size(); i++) {
        shifted_message += updated_message[i];
    }
    for (int i = 0; i < updated_message.size() - shift_num; i++) {
        shifted_message += updated_message[i];
    }

    std::string embedding_message = "";

    for (char c : shifted_message) {
        embedding_message += reverseString(intToBitString(static_cast<int>(c)));
    }

    int index_counter = 0;

    for (std::pair<int, int> p : positions) {
        if (index_counter == embedding_message.size()) {
            break;
        }
        else {
            //lsb is already 1
            if (static_cast<int>(encoded_image.get_data(p.first, p.second)) % 2 && embedding_message[index_counter] == '1') {
                index_counter += 1;
                continue;
            }
            //lsb is already 0
            else if (static_cast<int>(encoded_image.get_data(p.first, p.second)) % 2 == 0 && embedding_message[index_counter] == '0') {
                index_counter += 1;
                continue;
            }
            //lsb is one and message bit is 0. so decrease by one
            else if (static_cast<int>(encoded_image.get_data(p.first, p.second)) % 2 && embedding_message[index_counter] == '0') {
                encoded_image.set_data(encoded_image.get_data(p.first, p.second) - 1, p.first, p.second);
                if (encoded_image.get_data(p.first, p.second) < 0) {
                    encoded_image.set_data(0, p.first, p.second);
                }
                else if (encoded_image.get_data(p.first, p.second) > 255) {
                    encoded_image.set_data(255, p.first, p.second);
                }
            }
            //lsb is zero and message bit is 1. so increase by one
            else if (static_cast<int>(encoded_image.get_data(p.first, p.second)) % 2 == 0 && embedding_message[index_counter] == '1') {
                encoded_image.set_data(encoded_image.get_data(p.first, p.second) + 1, p.first, p.second);
                if (encoded_image.get_data(p.first, p.second) < 0) {
                    encoded_image.set_data(0, p.first, p.second);
                }
                else if (encoded_image.get_data(p.first, p.second) > 255) {
                    encoded_image.set_data(255, p.first, p.second);
                }
            }
        }
        index_counter += 1;
    }

    return encoded_image;

}

bool EncodeMessage::isPrime(const int& number) {
    if (number < 2) {
        return false;
    }
    if (number % 2 == 0) {
        return number == 2;
    }

    for (int i = 3; i < std::sqrt(number) + 1; i+= 2) {
        if (number % i == 0) {
            return false;
        }
    }
    return true;
}

int EncodeMessage::nthFibonacci(const int& n) {
    //because we are looking for prime indexed fibonacci numbers, we will start looking from 2nd number
    int first = 0, second = 1;

    for (int i = 2; i <= n; i++) {
        int temp = first;
        first = second;
        second += temp;
    }
    return second;
}


std::string EncodeMessage::intToBitString(int n) {
    std::string bitstring = "";

    while (n) {
        bitstring = std::to_string(n % 2) + bitstring;
        n /= 2;
    }
    if (bitstring.size() % 7) {
        int length = 7 - (bitstring.size() % 7);
        for (int i = 0; i < length; i++) {
            bitstring = "0" + bitstring;
        }
    }
    return bitstring;
}


std::string EncodeMessage::reverseString(std::string bitString) {

    for (int i = 0; i < bitString.size() / 2; i++) {
        char c = bitString[i];
        bitString[i] = bitString[bitString.size() - i - 1];
        bitString[bitString.size() - i - 1] = c;
    }
    return bitString;
}