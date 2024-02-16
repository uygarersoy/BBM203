#ifndef NETWORK_H
#define NETWORK_H

#include <fstream>
#include <sstream>
#include <ctime>

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"

using namespace std;

class Network {
public:
    Network();
    ~Network();
    
    vector<Client> client_list;
    
    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port, const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename); 

    vector<string> read_file(const string &file_name);
    vector<string> split(string line, char delimiter);
    void create_message(vector<Client> &clients, vector<string> &check, int message_limit, const string &sender_port, const string &receiver_port, string &command);
    string extract_message(vector<string> message);
    vector<string> split_message(string message, int message_limit);
    vector<int> get_client_indices(vector<Client> &clients, vector<string> &check);
    void show_q_info(vector<Client> &clients, vector<string> &check, string &command);
    void display_message_header(string &command, string &message);
    void show_frame_info(vector<Client> clients, vector<string> check, string command);
    void print_log(vector<Client> &clients, vector<string> &check, string &command);
    string get_time_stamp();
    string activity_type_to_string(ActivityType type);
    void send(vector<Client> &clients);
    void print_send(Client client, vector<Client> clients);
    int find_receiver_index(Client client, vector<Client> clients);
    vector<int> frame_count_for_messages(Client &client);
    void freePacketStack(stack<Packet*>& packetStack);
    void receive(vector<Client> &clients);
    string id_from_mac(vector<Client> clients, string mac);
    bool is_client_receiver(Client client, stack<Packet*> frame);
    string find_next_hop(Client client, stack<Packet*> frame, vector<Client> clients);

};

#endif  // NETWORK_H
