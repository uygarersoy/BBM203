#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game

    bool operator==(const Block& other) const {
        //incorrect dimensions
        if (shape.size() != other.shape.size() || shape[0].size() != other.shape[0].size()) {
            return false;
        }

        //element-wise comparison
        for (int i = 0; i < shape.size(); i++) {
            for (int j = 0; j < shape[0].size(); j++) {
                if (shape[i][j] != other.shape[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Block& other) const {
        //use negation and overloaded == operation as a shortcut
        return !(*this == other);
    }
};


#endif //PA2_BLOCK_H
