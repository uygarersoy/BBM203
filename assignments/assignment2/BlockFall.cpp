#include "BlockFall.h"

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    
    vector<vector<vector<bool>>> blocks = createBlocksFromFile(input_file);
    power_up = blocks[blocks.size() - 1];
    blocks.pop_back();
    Block* head = new Block;
    head->shape = blocks[0];
    initial_block = head;
    vector<Block *> gameBlocks = {linkBlocks(rotations(initial_block))};

    for (int i = 1; i < blocks.size(); i++) {
        Block* temp = new Block;
        temp->shape = blocks[i];
        vector<Block*> rotatedBlocks = rotations(temp);
        Block* linkedBlock = linkBlocks(rotatedBlocks);
        gameBlocks.push_back(linkedBlock);
    }

    for (int i = 0; i < gameBlocks.size() - 1; i++) {
        Block * tempHead = gameBlocks[i];
        tempHead->next_block = gameBlocks[i+1];
        Block* linkRotation = tempHead->right_rotation;
        while(*linkRotation != *gameBlocks[i]) {
            linkRotation->next_block = gameBlocks[i+1];
            linkRotation = linkRotation->right_rotation;
        }
    }

    active_rotation = initial_block;
}

void BlockFall::initialize_grid(const string &input_file) {

    vector<vector<int>> temp_grid;

    ifstream file(input_file);

    if (!file.is_open()) {
        cerr << "An error has occurred while trying to open the file" << endl;
        exit(-1);
    }
    string line;
    while(getline(file, line)) {
        vector<int> row;
        istringstream iss(line);
        int element;

        while (iss >> element) {
            row.push_back(element);
        }
        temp_grid.push_back(row);
    }
    file.close();

    rows = temp_grid.size();
    cols = temp_grid[0].size();
    grid = temp_grid;
}

vector<vector<vector<bool>>> BlockFall::createBlocksFromFile(const string& input_file) {
    vector<vector<vector<bool>>> allBlocks;
    ifstream file(input_file);

    if (!file.is_open()) {
        cerr << "An error occurred while trying to open the file!";
        exit(-1);
    }

    vector<vector<bool>> currentMatrix;
    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        else if (line[0] == '[' || line[0] == '1' || line[0] == '0') {
            char item;
            vector<bool> tempRow;
            istringstream ss(line);

            while (ss >> item) {
                if (item == '0') {
                    tempRow.push_back(false);
                }
                else if (item == '1') {
                    tempRow.push_back(true);
                }
            }
            currentMatrix.push_back(tempRow);
            if (line[line.size()-1] == ']') {
                allBlocks.push_back(currentMatrix);
                currentMatrix.clear();
            }
        }
    }
    file.close();
    return allBlocks;
}

vector<vector<int>> BlockFall::transpose(vector<vector<int> > matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<vector<int>> result(cols, vector<int>(rows));

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result[j][i] = matrix[i][j];
        }
    }

    return result;
}

vector<vector<int> > BlockFall::reverseRows(vector<vector<int>> matrix) {
    int rowSize = matrix.size();
    vector<vector<int> > result = matrix;

    for (int i = 0; i < rowSize / 2; i++) {
        swap(result[i], result[rowSize - i - 1]);
    }
    return result;
}

vector<vector<bool> > BlockFall::rotateMatrix90Degrees(vector<vector<bool>> matrix) {
    vector<vector<int>> convMatrix = convertVectorToInt(matrix);
    vector<vector<int> > reversed = reverseRows(convMatrix);
    vector<vector<int> > rotated = transpose(reversed);
    vector<vector<bool>> convMatrixShape = convertVectorToBool(rotated);
    return convMatrixShape;
}
vector<vector<int>> BlockFall::convertVectorToInt(vector<vector<bool>> inputVec) {
    vector<vector<int>> converted;
    for (vector<bool> vec : inputVec) {
        vector<int> temp;
        for (bool b : vec) {
            temp.push_back(b ? 1: 0);
        }
        converted.push_back(temp);
    }
    return converted;
}

vector<vector<bool>> BlockFall::convertVectorToBool(vector<vector<int>> inputVec) {
    vector<vector<bool>> converted;
    for (vector<int> vec : inputVec) {
        vector<bool> temp;
        for (int i : vec) {
            temp.push_back(i ? 1: 0);
        }
        converted.push_back(temp);
    }
    return converted;
}
vector<Block *> BlockFall::rotations(Block * block) {
    vector<Block *> rotatedBlocks = {block};

    while (true) {
        Block * lastBlock = rotatedBlocks[rotatedBlocks.size()-1];
        Block * temp = new Block;
        temp->shape = rotateMatrix90Degrees(lastBlock->shape);
        if (*temp == *block) {
            delete temp;
            break;
        }
        else {
            rotatedBlocks.push_back(temp);
        }
    }
    return rotatedBlocks;
}


Block * BlockFall::linkBlocks(const vector<Block *> &blocksVector) {
    if (blocksVector.size() == 1) {
        Block * mainBlock = blocksVector[0];
        mainBlock->right_rotation = mainBlock;
        mainBlock->left_rotation = mainBlock;
        return mainBlock;
    }
    else {
        Block * head = blocksVector[0];
        head->left_rotation = blocksVector[blocksVector.size() -1];
        head->right_rotation = blocksVector[1];
        for (int i = 1; i < blocksVector.size() - 1; i++) {
            blocksVector[i]->left_rotation = blocksVector[i-1];
            blocksVector[i]->right_rotation = blocksVector[i+1];
        }
        blocksVector[blocksVector.size() -1]->left_rotation = blocksVector[blocksVector.size() -2];
        blocksVector[blocksVector.size() -1]->right_rotation = head;
        return head;
    }
}

BlockFall::~BlockFall() {

    while (initial_block != nullptr) {
        Block * next = initial_block->next_block;
        if (*(initial_block->right_rotation) == *(initial_block)) {
            delete initial_block;
        }
        else {
            Block* temp = initial_block->right_rotation;
            while (*(temp) != *(initial_block)) {
                Block* right = temp->right_rotation;
                delete temp;
                temp = right;
            }
            delete initial_block;
        }

        initial_block = next;
    }
}