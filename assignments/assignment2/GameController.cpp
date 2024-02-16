#include "GameController.h"

bool GameController::play(BlockFall& game, const string& commands_file){

    vector<string> lines = read_file(commands_file);
    Block* saver = game.initial_block;
    int curr_row = 0, curr_col = 0;

    for (string command : lines) {
        if (game.initial_block == nullptr) {
            game.initial_block = saver;
            LeaderboardEntry* curr = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
            game.leaderboard.insert_new_entry(curr);
            game.leaderboard.write_to_file(game.leaderboard_file_name);
            no_more_block(game.grid, game.current_score, game.leaderboard);
            return true;
        }
        if (!check_can_fit(game.initial_block, game.grid)) {
            LeaderboardEntry* curr = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
            game.leaderboard.insert_new_entry(curr);
            game.leaderboard.write_to_file(game.leaderboard_file_name);
            //dont_fit(game.grid, game.initial_block->shape, game.current_score, game.leaderboard);
            game.initial_block = saver;
            return false;
        }
        if (command == "GRAVITY_SWITCH") {
            if (!game.gravity_mode_on) {
                update_grid(game.grid);
                clear_rows(game.grid, game.current_score, false);
            }
            game.gravity_mode_on = !game.gravity_mode_on;
        }
        else if (command == "PRINT_GRID") {
            print_grid(game.grid, game.initial_block, curr_row, curr_col, game.current_score, game.leaderboard);
        }
        else if (command == "ROTATE_RIGHT") {
            if (canRotateRight(game.grid, game.initial_block, curr_row, curr_col)) {
                game.initial_block = game.initial_block->right_rotation;
                game.active_rotation = game.initial_block;
            }
        }
        else if (command == "ROTATE_LEFT") {
            if (canRotateLeft(game.grid, game.initial_block, curr_row, curr_col)) {
                game.initial_block = game.initial_block->left_rotation;
                game.active_rotation = game.initial_block;
            }
        }
        else if (command == "MOVE_LEFT") {
            moveLeft(game.grid, game.initial_block, curr_row, curr_col);
        }
        else if (command == "MOVE_RIGHT") {
            moveRight(game.grid, game.initial_block, curr_row, curr_col);
        }
        else if (command == "DROP") {
            if (game.gravity_mode_on) {
                drop_gravity_on(game.grid, game.initial_block, curr_row, curr_col, game.current_score, game.power_up);
                game.initial_block = game.initial_block->next_block;
                game.active_rotation = game.initial_block;
                curr_row = 0; curr_col = 0;   
            }
            else{
                drop(game.grid, game.initial_block, curr_row, curr_col, game.current_score, game.power_up);
                game.initial_block = game.initial_block->next_block;
                game.active_rotation = game.initial_block;
                curr_row = 0; curr_col = 0;
            }
               
        }
        else {
            cout << "Unknown command: " << command << "\n";
        }

    }
    game.initial_block = saver;
    LeaderboardEntry* curr = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
    game.leaderboard.insert_new_entry(curr);
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    no_more_commands(game.grid, game.current_score, game.leaderboard);

    return true;
}

vector<string> GameController::read_file(const string &file_name) {
    ifstream file(file_name);
    vector<string> commands;
    string line;
    if (!file.is_open()) {
        cerr << "An error occurred while trying to open the file!";
        return commands;
    }

    while (getline(file, line)) {
        commands.push_back(line);
    }
    file.close();
    return commands;
}

bool GameController::check_can_fit(Block* &curr, vector<vector<int>> &game_grid) {
    for (int i = 0; i < curr->shape.size(); i++) {
        for (int j = 0; j < curr->shape[i].size(); j++) {
            if (game_grid[i][j] == 1 && curr->shape[i][j]) {
                return false;
            }
        }
    }
    return true;
}


void GameController::moveRight(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col) {
    if (checkMoveRight(game_grid, current_block, curr_row, curr_col)) {
        curr_col++;
    }
}

bool GameController::checkMoveRight(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col) {
    if (curr_col + current_block->shape[0].size() >= game_grid[0].size()) {
        return false;
    }
    int check_col = curr_col + 1;
    vector<vector<bool>> tempBlock;

    for (int i = curr_row; i < curr_row + current_block->shape.size(); i++) {
        vector<bool> temp;
        for (int j = check_col; j < check_col + current_block->shape[0].size(); j++) {
            temp.push_back(game_grid[i][j] == 1);
        }
        tempBlock.push_back(temp);
    }

    for (int i = 0; i < current_block->shape.size(); i++) {
        for (int j = 0; j < current_block->shape[0].size(); j++) {
            if (current_block->shape[i][j] && tempBlock[i][j]) {
                return false;
            }
        }
    }
    return true;
}

void GameController::moveLeft(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col) {
    if (checkMoveLeft(game_grid, current_block, curr_row, curr_col)) {
        curr_col--;
    }
}

bool GameController::checkMoveLeft(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col) {
    if (curr_col - 1 < 0) {
        return false;
    }

    int check_col = curr_col - 1;
    vector<vector<bool>> tempBlock;

    for (int i = curr_row; i < curr_row + current_block->shape.size(); i++) {
        vector<bool> temp;
        for (int j = check_col; j < check_col + current_block->shape[0].size(); j++) {
            temp.push_back(game_grid[i][j] == 1);
        }
        tempBlock.push_back(temp);
    }

    for (int i = 0; i < current_block->shape.size(); i++) {
        for (int j = 0; j < current_block->shape[0].size(); j++) {
            if (current_block->shape[i][j] && tempBlock[i][j]) {
                return false;
            }
        }
    }

    return true;
}

bool GameController::canRotateRight(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col) {
    Block* rotatedBlock = current_block->right_rotation;

    if (curr_row + rotatedBlock->shape.size() > game_grid.size() || curr_col + rotatedBlock->shape[0].size() > game_grid[0].size()) {
        return false;
    }

    for (int i = 0; i < rotatedBlock->shape.size(); i++) {
        for (int j = 0; j < rotatedBlock->shape[0].size(); j++) {
            if (rotatedBlock->shape[i][j] && game_grid[curr_row + i][curr_col + j] == 1) {
                return false;
            }
        }
    }

    return true;
}

bool GameController::canRotateLeft(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col) {
    Block* rotatedBlock = current_block->left_rotation;

    if (curr_row + rotatedBlock->shape.size() > game_grid.size() || curr_col + rotatedBlock->shape[0].size() > game_grid[0].size()) {
        return false;
    }

    for (int i = 0; i < rotatedBlock->shape.size(); i++) {
        for (int j = 0; j < rotatedBlock->shape[0].size(); j++) {
            if (rotatedBlock->shape[i][j] == 1 && game_grid[curr_row + i][curr_col + j] == 1) {
                return false;
            }
        }
    }

    return true;
}

void GameController::print_grid(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col, unsigned long &score, Leaderboard &leader) {
    int grid_row = game_grid.size();
    int grid_col = game_grid[0].size();
    LeaderboardEntry * head = leader.head_leaderboard_entry;
    vector<vector<int>> temp_grid(grid_row, vector<int>(grid_col, 0));

    for (int i = 0; i < grid_row; i++) {
        for (int j = 0; j < grid_col; j++) {
            temp_grid[i][j] = game_grid[i][j];
        }
    }

    for (int i = 0; i < current_block->shape.size(); i++) {
        for (int j = 0; j < current_block->shape[0].size(); j++) {
            if (current_block->shape[i][j]) {
                int printed_row = curr_row + i;
                int printed_col = curr_col + j;

                if (printed_row >= 0 && printed_row < grid_row && printed_col >= 0 && printed_col < grid_col) {
                    temp_grid[printed_row][printed_col] = 1;
                }
            }
        }
    }
    int highscore;
    if (head == nullptr || score > head->score) {
        highscore = score;
    }
    else {
        highscore = head->score;
    }
    cout << "Score: " << score;
    cout << "\nHigh Score: " << highscore << "\n";
    for (vector<int> vec : temp_grid) {
        for (int i : vec) {
            if (i == 1) {
                cout << "██";
            }
            else {
                cout << "▒▒";
            }
        }
        cout << "\n";
    }
    cout <<"\n\n";
    
}
void GameController::drop(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col, unsigned long &current_score, vector<vector<bool>> &power_up) {

    int temp_row = curr_row;
    bool dropped = false;
    int count = count_of_full_cells(current_block);

    while (!dropped && (temp_row < game_grid.size() - current_block->shape.size() + 1)) {
        vector<vector<bool>> compare_block;
        for (int i = temp_row; i < temp_row + current_block->shape.size(); i++) {
            vector<bool> temp;
            for (int j = curr_col; j < curr_col + current_block->shape[0].size(); j++) {
                if (game_grid[i][j] == 1) {
                    temp.push_back(true);
                }
                else{
                    temp.push_back(false);
                }
            }
            compare_block.push_back(temp);
        }
        bool collision = false;

        for (int i = 0; i < current_block->shape.size(); i++) {
            for (int j = 0; j < current_block->shape[i].size(); j++) {
                if (compare_block[i][j] && current_block->shape[i][j]) {
                    collision = true;
                    break;
                }
            }
            if (collision) {
                break;
            }
        }
        int row = 0, col = 0;
        if (collision) {
            for (int i = temp_row - 1; i < temp_row - 1 + current_block->shape.size(); i++) {
                for (int j = curr_col; j < curr_col + current_block->shape[0].size(); j++) {
                    if (current_block->shape[row][col]) {
                        game_grid[i][j] = 1;
                    }
                    col++;
                }
                row++;
                col = 0;
            }
            current_score += (count * (temp_row - 1));
            dropped = true;
            break;
        }
        temp_row++;
    }
    if (!dropped) {
        int row = 0, col = 0;
        for (int i = game_grid.size() - current_block->shape.size(); i < game_grid.size(); i++) {
            for (int j = curr_col; j < curr_col + current_block->shape[0].size(); j++) {
                if (current_block->shape[row][col]) {
                    game_grid[i][j] = 1;
                }
                col++;
            }
            row++;
            col = 0;
        }
        current_score += (count * (game_grid.size() - current_block->shape.size()));

    }
    //check_power_up(game_grid, power_up, current_score);
    clear_rows(game_grid, current_score, true);
}

void GameController::drop_gravity_on(vector<vector<int>> &game_grid, Block* &current_block, int &curr_row, int &curr_col, unsigned long &current_score, vector<vector<bool>> &power_up) {
    int temp_col = curr_col;
    int square_count = 0;
    int min_dist = game_grid.size();

    while (temp_col < curr_col + current_block->shape[0].size()) {
        int deepest_one = 0;
        int solid_count = 0;
        int nearest_full_cell = game_grid.size();

        for (int i = 0; i < current_block->shape.size(); i++) {
            if (current_block->shape[i][temp_col - curr_col]) {
                deepest_one = i;
                solid_count++;
                square_count++;
            }
        }

        for (int i = deepest_one; i < game_grid.size(); i++) {
            if (game_grid[i][temp_col] == 1) {
                nearest_full_cell = i;
                break;
            }
        }

        int dist_to_fall = nearest_full_cell - solid_count;
        if (dist_to_fall < min_dist) {
            min_dist = dist_to_fall;
        }
        for (int i = dist_to_fall; i < nearest_full_cell; i++) {
            game_grid[i][temp_col] = 1;
        }
        temp_col++;
    }

    current_score += (square_count * min_dist);
    check_power_up(game_grid, power_up, current_score);
    clear_rows(game_grid, current_score, true);
}



void GameController::no_more_block(vector<vector<int>> &game_grid, unsigned long &score, Leaderboard &leader) {
    cout << "YOU WIN!\nNo more blocks.\nFinal grid and score:\n\nScore: " << score << "\n" << "Highscore: ";
    int highscore;
    LeaderboardEntry* head = leader.head_leaderboard_entry;

    if (head == nullptr || score > head->score) {
        highscore = score;
    }
    else {
        highscore = head->score;
    }
    cout << highscore << "\n";
    for (vector<int> vec : game_grid) {
        for (int i : vec) {
            if (i == 1) {
                cout << "██";
            }
            else {
                cout << "▒▒";
            }
        }
        cout << "\n";
    }
    cout <<"\n\n";
    leader.print_leaderboard();
}

int GameController::count_of_full_cells(Block* &current_block) {
    int count = 0;
    for (int i = 0; i < current_block->shape.size(); i++) {
        for (int j = 0; j < current_block->shape[i].size(); j++) {
            if (current_block->shape[i][j]) {
                count++;
            }
        }
    }
    return count;
}

void GameController::no_more_commands(vector<vector<int>> &game_grid, unsigned long &score, Leaderboard &leader) {
    cout << "GAME FINISHED!\nNo more commands.\nFinal grid and score:\n\nScore: " << score << "\nHigh Score: ";
    int highscore;
    LeaderboardEntry* head = leader.head_leaderboard_entry;

    if (head == nullptr || score > head->score) {
        highscore = score;
    }
    else {
        highscore = head->score;
    }
    cout << highscore << "\n";
    for (vector<int> vec : game_grid) {
        for (int i : vec) {
            if (i == 1) {
                cout << "██";
            }
            else {
                cout << "▒▒";
            }
        }
        cout << "\n";
    }
    cout <<"\n\n";
    leader.print_leaderboard();
}

void GameController::dont_fit(vector<vector<int>> &game_grid, vector<vector<bool>> &block, unsigned long &score, Leaderboard &leader) {
    cout << "GAME OVER!\nNext block that couldn't fit:\n";
    for (int i = 0; i < block.size(); i++) {
        for (int j = 0; j < block[i].size(); j++) {
            if (block[i][j]) {
                cout << "██";
            }
            else {
                cout << "▒▒";
            }
        }
        cout << "\n";
    }

    
    cout << "\n\nFinal grid and score:\n\nScore: " << score << "\nHigh score: ";
    int highscore = 0;
    LeaderboardEntry* head = leader.head_leaderboard_entry;

    if (head == nullptr) {
        highscore = score;
    }
    else if (score > head->score) {
        highscore = score;
    }
    else {
        highscore = head->score;
    }
    cout << highscore << "\n";
    for (vector<int> vec : game_grid) {
        for (int i : vec) {
            if (i == 1) {
                cout << "██";
            }
            else {
                cout << "▒▒";
            }
        }
        cout << "\n";
    }
    cout <<"\n\n";
    leader.print_leaderboard();
}

void GameController::update_grid(vector<vector<int>> &game_grid) {
    for (int i = 0; i < game_grid[0].size(); i++) {
        int full_count = 0;
        for (int j = 0; j  < game_grid.size(); j++) {
            if (game_grid[j][i] == 1) {
                full_count++;
            }
        }
        for (int k = 0; k < game_grid.size(); k++) {
            if (k < game_grid.size() - full_count) {
                game_grid[k][i] = 0;
            }
            else {
                game_grid[k][i] = 1;
            }
        }
    }
}

void GameController::clear_rows(vector<vector<int>> &game_grid, unsigned long &score, bool parameter) {
    int row_count = 0;
    vector<int> rows;
    for (int i = 0; i < game_grid.size(); i++) {
        int count = 0;
        for (int j = 0; j < game_grid[0].size(); j++) {
            if (game_grid[i][j] == 1) {
                count++;
            }
        }
        if (count == game_grid[0].size()) {
            rows.push_back(i);
            row_count++;
        }
    }
    if (row_count != 0) {
        if (parameter) {
            cout << "Before clearing:\n";
            for (vector<int> vec : game_grid) {
                for (int i : vec) {
                    if (i == 1) {
                        cout << "██";
                    }
                    else {
                        cout << "▒▒";
                    }
                }
                cout << "\n";
            }
            cout <<"\n\n";
        }
        
    }
    vector<vector<int>> cleaned_rows;
    for (int i = 0; i < row_count; i++) {
        vector<int> temp;
        for (int j = 0; j < game_grid[0].size(); j++) {
            temp.push_back(0);
        }
        cleaned_rows.push_back(temp);
    }


    for (int i = 0; i < game_grid.size(); i++) {
        bool inside = false;
        for (int row : rows) {
            if (i == row) {
                inside = true;
                break;
            }
        }
        if (!inside) {
            cleaned_rows.push_back(game_grid[i]);
        }
    }
    for (int i = 0; i < game_grid.size(); i++) {
        for (int j = 0; j < game_grid[0].size(); j++) {
            game_grid[i][j] = cleaned_rows[i][j];
        }
    }
    score += (row_count * game_grid[0].size());
}

void GameController::check_power_up(vector<vector<int>> &game_grid, vector<vector<bool>> &power_up, unsigned long &score) {
    bool found = false;
    for (int i = 0; i <= game_grid.size() - power_up.size(); i++) {
        for (int j = 0; j <= game_grid[0].size() - power_up[0].size(); j++) {
            vector<vector<bool>> temp_section;
            for (int k = i; k < i + power_up.size(); k++) {
                vector<bool> temp;
                for (int l = j; l < j + power_up[0].size(); l++) {
                    if (game_grid[k][l] == 1) {
                        temp.push_back(true);
                    }
                    else {
                        temp.push_back(false);
                    }
                }
                temp_section.push_back(temp);
            }
            bool match = true;
            for (int x = 0; x < power_up.size(); x++) {
                for (int y = 0; y < power_up[x].size(); y++) {
                    if (power_up[x][y] != temp_section[x][y]) {
                        match = false;
                    }
                }
            }
            if (match) {
                found = true;
                cout << "Before clearing:\n";
                for (vector<int> vec : game_grid) {
                    for (int i : vec) {
                        if (i == 1) {
                            cout << "██";
                        }
                        else {
                            cout << "▒▒";
                        }
                    }
                    cout << "\n";
                }
                cout <<"\n\n";
                int square = 0;
                for (int r = 0; r < game_grid.size(); r++) {
                    for (int c = 0; c < game_grid[r].size(); c++) {
                        if (game_grid[r][c] == 1) {
                            square++;
                        }
                        game_grid[r][c] = 0;
                    }
                }
                score += 1000 + square;
                break;
            }
            if (found) {
                break;
            }
        }
        if (found) {
            break;
        }
    }
}