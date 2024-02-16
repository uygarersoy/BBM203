#ifndef PA2_LEADERBOARD_H
#define PA2_LEADERBOARD_H

#include <ctime>
#include <string>
#include "LeaderboardEntry.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#define MAX_LEADERBOARD_SIZE 10

using namespace std;

class Leaderboard {
public:
    LeaderboardEntry* head_leaderboard_entry = nullptr;
    void read_from_file(const string &filename);
    void write_to_file(const string &filename);
    void print_leaderboard();
    void insert_new_entry(LeaderboardEntry *new_entry);
    vector<LeaderboardEntry*> read_file(string file_name);
    vector<string> split(string line, char delimiter);
    time_t stringToTimeT(const string& timestampString);
    string format_time_for_printing(time_t timestamp);
    void write_string_to_file(string file_name, string line);
    int linked_list_size(LeaderboardEntry* head);
    void pop_last_entry(LeaderboardEntry* head);
    virtual ~Leaderboard();
};


#endif //PA2_LEADERBOARD_H
