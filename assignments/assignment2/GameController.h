#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay
    vector<string> read_file(const string &file_name);
    bool check_can_fit(Block* &curr, vector<vector<int>> &game_grid);
    void moveRight(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col);
    bool checkMoveRight(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col);
    void moveLeft(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col);
    bool checkMoveLeft(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col);
    bool canRotateRight(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col);
    bool canRotateLeft(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col);
    void print_grid(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col, unsigned long &score, Leaderboard &leader);
    void drop(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col, unsigned long &current_score, vector<vector<bool>> &power_up);
    void no_more_block(vector<vector<int>> &game_grid, unsigned long &score, Leaderboard &leader);
    int count_of_full_cells(Block* &current_block);
    void no_more_commands(vector<vector<int>> &game_grid, unsigned long &score, Leaderboard &leader);
    void dont_fit(vector<vector<int>> &game_grid, vector<vector<bool>> &block, unsigned long &score, Leaderboard &leader);
    void drop_gravity_on(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col, unsigned long &current_score, vector<vector<bool>> &power_up);
    void update_grid(vector<vector<int>> &game_grid);
    void clear_rows(vector<vector<int>> &game_grid, unsigned long &score, bool parameter);
    void check_power_up(vector<vector<int>> &game_grid, vector<vector<bool>> &power_up, unsigned long &score);


};


#endif //PA2_GAMECONTROLLER_H
