#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    if (head_leaderboard_entry == nullptr || head_leaderboard_entry->score < new_entry->score) {
        new_entry->next_leaderboard_entry = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    }
    else {
        LeaderboardEntry* prev = nullptr;
        LeaderboardEntry* curr = head_leaderboard_entry;

        while (curr != nullptr && (curr->score > new_entry->score || (curr->score == new_entry->score && curr->last_played <= new_entry->last_played))) {
            prev = curr;
            curr = curr->next_leaderboard_entry;
        }
        new_entry->next_leaderboard_entry = curr;
        prev->next_leaderboard_entry = new_entry;
    }
    if (linked_list_size(head_leaderboard_entry) > MAX_LEADERBOARD_SIZE) {
        pop_last_entry(head_leaderboard_entry);
    }
}

void Leaderboard::write_to_file(const string& filename) {
    string line;
    LeaderboardEntry* temp = head_leaderboard_entry;
    while (temp) {
        line += to_string(temp->score) + " " + to_string(temp->last_played) + " " + temp->player_name + "\n";
        temp = temp->next_leaderboard_entry;
    }

    line.pop_back();
    write_string_to_file(filename, line);  
}

void Leaderboard::read_from_file(const string& filename) {

    vector<LeaderboardEntry*> entries = read_file(filename);
    if (entries.size() != 0) {
        head_leaderboard_entry = entries[0];
    
        LeaderboardEntry* temp = head_leaderboard_entry;

        for (int i = 1; i < entries.size(); i++) {
            temp->next_leaderboard_entry = entries[i];
            temp = temp->next_leaderboard_entry;
        }
    }
}


void Leaderboard::print_leaderboard() {
    int index = 1;
    cout << "Leaderboard\n-----------\n";
    LeaderboardEntry* temp = head_leaderboard_entry;
    while (temp != nullptr) {
        string formatted_time = format_time_for_printing(temp->last_played);
        cout << index << ". " << temp->player_name << " " << temp->score << " " << formatted_time << "\n";
        index++;
        temp = temp->next_leaderboard_entry;
    }
}

vector<LeaderboardEntry*> Leaderboard::read_file(string file_name)
{
    ifstream file(file_name);
    vector<LeaderboardEntry*> entries;
    if (!file.is_open())
    {
        cerr << "File did not open!" << endl;
        return entries;
    }

    else {
        string line;

        while (getline(file, line))
        {
            vector<string> temp = split(line, ' ');
            unsigned long score = stoul(temp[0]);
            time_t last_played = stringToTimeT(temp[1]);
            string player_name = temp[2];
            LeaderboardEntry* temp_entry = new LeaderboardEntry(score, last_played, player_name);
            entries.push_back(temp_entry);
        }
        return entries;
    }    
}
vector<string> Leaderboard::split(string line, char delimiter)
{
    vector<string> vec;
    string temp;
    stringstream ss(line);

    while (getline(ss, temp, delimiter))
    {
        vec.push_back(temp);
    }
    return vec;
}


time_t Leaderboard::stringToTimeT(const string& timestampString) {
    
    long long timestamp = stoll(timestampString);
    return static_cast<time_t>(timestamp);
}

string Leaderboard::format_time_for_printing(time_t timestamp) {
    struct tm* formatted_time = localtime(&timestamp);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S/%d.%m.%Y", formatted_time);
    return string(buffer);
}

void Leaderboard::write_string_to_file(string file_name, string line) {
    ofstream file(file_name);
    if (!file.is_open()) {
        cerr << "An error happened while trying to open the file!";
        exit(-1);
    }

    file << line;
    file.close();
}

int Leaderboard::linked_list_size(LeaderboardEntry* head) {
    int size = 0;
    LeaderboardEntry* curr = head;
    while(curr) {
        ++size;
        curr = curr->next_leaderboard_entry;
    } 
    return size;
}

void Leaderboard::pop_last_entry(LeaderboardEntry* head) {
    LeaderboardEntry* curr = head;
    LeaderboardEntry* prev = nullptr;
    while (curr->next_leaderboard_entry != nullptr) {
        prev = curr;
        curr = curr->next_leaderboard_entry;
    }
    delete curr;
    prev->next_leaderboard_entry = nullptr;
}

Leaderboard::~Leaderboard() {
    LeaderboardEntry* temp = head_leaderboard_entry;

    while (temp != nullptr) {
        LeaderboardEntry* next = temp->next_leaderboard_entry;
        delete temp;
        temp = next;
    }
}
