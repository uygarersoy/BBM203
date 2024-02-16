#include "Network.h"

Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port, const string &receiver_port) {
    for (string comm : commands) {
        vector<string> check = split(comm, ' ');

        if (check[0] == "MESSAGE") {
            create_message(clients, check, message_limit, sender_port, receiver_port, comm);
        }
        else if (check[0] == "SHOW_Q_INFO") {
            show_q_info(clients, check, comm);
        }
        else if (check[0] == "SHOW_FRAME_INFO") {
            show_frame_info(clients, check, comm);
        }   
        else if (check[0] == "SEND") {
            send(clients);
        }
        else if (check[0] == "RECEIVE") {
            receive(clients);
        }
        else if (check[0] == "PRINT_LOG") {
            print_log(clients, check, comm);
        }
        else {
            string dashes;
            int dash_count = 9 + comm.size();
            for (int i = 0; i < dash_count; i++) {
                dashes += "-";
            }
            cout << dashes << "\nCommand: " << comm << "\n" << dashes << "\nInvalid command.\n"; 
        }
    }

}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    vector<string> lines = read_file(filename);
    if (lines.size() == 0) {
        return clients;
    }
    int client_number = stoi(lines[0]);
    for (int i = 1; i <= client_number; i++) {
        string client_id, client_ip, client_mac;
        vector<string> client_info = split(lines[i], ' ');
        client_id = client_info[0]; client_ip = client_info[1]; client_mac = client_info[2];
        Client client(client_id, client_ip, client_mac);
        clients.push_back(client);
    }
    client_list = clients;
    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    vector<string> lines = read_file(filename);
    int index = 0;

    for (Client &client : clients) {
        while (index < lines.size()) {
            if (lines[index] == "-") {
                index++;
                break;
            }
            vector<string> pairs = split(lines[index], ' ');
            client.routing_table[pairs[0]] = pairs[1];
            index++;
        }
    }
}

vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    vector<string> lines = read_file(filename);
    if (lines.size() == 0) {
        return commands;
    }
    int command_count = stoi(lines[0]);

    for (int i = 1; i < command_count + 1; i++) {
        commands.push_back(lines[i]);
    }
    return commands;
}

vector<string> Network::read_file(const string &file_name)
{
    vector<string> lines;
    ifstream file(file_name);
    if (!file.is_open()) {
        cerr << "File did not open!" << endl;
        exit(-1);
    }

    string line;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}
vector<string> Network::split(string line, char delimiter)
{
    vector<string> vec;
    string temp;
    stringstream ss(line);

    while (getline(ss, temp, delimiter)) {
        vec.push_back(temp);
    }
    return vec;
}

void Network::create_message(vector<Client> &clients, vector<string> &check, int message_limit, const string &sender_port, const string &receiver_port, string &command) {
    string message = extract_message(check);
    vector<string> message_frames = split_message(message, message_limit);
    vector<int> three_client = get_client_indices(clients, check);
    string timestamp = get_time_stamp();

    ActivityType type = ActivityType::MESSAGE_FORWARDED;
    Log log(timestamp, message, message_frames.size(), 0, clients[three_client[0]].client_id, clients[three_client[1]].client_id, true, type);
    clients[three_client[0]].log_entries.push_back(log);

    int frame_count = 1;
    display_message_header(command, message);

    for (string chunk : message_frames) {
        stack<Packet *> frame;
        ApplicationLayerPacket* app = new ApplicationLayerPacket(0, check[1], check[2], chunk);
        frame.push(app);
        TransportLayerPacket* transport = new TransportLayerPacket(1, sender_port, receiver_port);
        frame.push(transport);
        NetworkLayerPacket* net = new NetworkLayerPacket(2, clients[three_client[0]].client_ip, clients[three_client[1]].client_ip);
        frame.push(net);
        PhysicalLayerPacket* physical = new PhysicalLayerPacket(3, clients[three_client[0]].client_mac, clients[three_client[2]].client_mac);
        frame.push(physical);
        clients[three_client[0]].outgoing_queue.push(frame);
        cout << "Frame #" << frame_count << "\n";
        physical->print();
        net->print();
        transport->print();
        app->print();
        cout << "Message chunk carried: \"" << chunk << "\"\n";
        cout << "Number of hops so far: " << log.number_of_hops << "\n";
        string dashes;
        int dash_count = 7 + to_string(frame_count).size();
        for (int i = 0; i < dash_count; i++) {
            dashes += "-";
        }
        cout << dashes << endl;
        frame_count++;
    }

}

string Network::get_time_stamp() {

    time_t currentTime = time(nullptr);
    tm* localTime = localtime(&currentTime);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localTime);
    string formattedTime(buffer);
    return formattedTime;
}

void Network::display_message_header(string &command, string &message) {
    int dash_size = 9 + command.size();
    string dashes;
    for (int i = 0; i < dash_size; i++) {
        dashes += "-";
    }
    cout << dashes << "\nCommand: " << command << "\n" << dashes << "\nMessage to be sent: \"" << message << "\"\n\n";
}

string Network::extract_message(vector<string> message) {
    string res;
    for (int i = 3; i < message.size(); i++) {
        for (char c : message[i]) {
            res += c;
        }
        res += " ";
    }
    string final_message;
    for (int i = 1; i < res.size() -2; i++) {
        final_message += res[i];
    }
    return final_message;
}

vector<string> Network::split_message(string message, int message_limit) {
    vector<string> result;
    string temp;

    for (char c: message) {
        if (temp.size() != message_limit) {
            temp += c;
        }
        else {
            result.push_back(temp);
            temp = c;
        }
    }
    if (temp.size() != 0) {
        result.push_back(temp);
    }
    return result;
}

vector<int> Network::get_client_indices(vector<Client> &clients, vector<string> &check) {
    vector<int> client_indices;
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].client_id == check[1]) {
            client_indices.push_back(i);
            break;
        }
    }
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].client_id == check[2]) {
            client_indices.push_back(i);
            break;
        }
    }

    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].client_id == clients[client_indices[0]].routing_table[clients[client_indices[1]].client_id]) {
            client_indices.push_back(i);
            break;
        }
    }

    return client_indices;
}

void Network::show_q_info(vector<Client> &clients, vector<string> &check, string &command) {
    int dash_size = 9 + command.size();
    queue<stack<Packet*>> out_queue;
    queue<stack<Packet*>> in_queue;
    for (Client c : clients) {
        if (c.client_id == check[1]) {
            out_queue = c.outgoing_queue;
            in_queue = c.incoming_queue;
            break;
        }
    }
    string dashes;
    int queue_size = check[2] == "out" ? out_queue.size() : in_queue.size();
    string queue = check[2] == "out" ? "Outgoing Queue Status" : "Incoming Queue Status";
    for (int i = 0; i < dash_size; i++) {
        dashes += "-";
    }
    cout << dashes << "\n" << "Command: " << command << "\n" << dashes << "\n" << "Client " << check[1] << " " << queue << "\n";
    cout << "Current total number of frames: " << queue_size << endl;
}

void Network::show_frame_info(vector<Client> clients, vector<string> check, string command) {
    queue<stack<Packet*>> out_queue;
    queue<stack<Packet*>> in_queue;  
    int dash_size = 9 + command.size();
    vector<Log> log;
    int log_index = -1;

    for (Client c : clients) {
        if (c.client_id == check[1]) {
            out_queue = c.outgoing_queue;
            in_queue = c.incoming_queue;
            log = c.log_entries;
            break;
        }
    }
    int temp = 0;
    for (int i = 0; i < log.size(); i++) {
        temp += log[i].number_of_frames;
        if (temp >= stoi(check[3])) {
            log_index = i;
            break;
        }
    }

    queue<stack<Packet*>> main_queue = check[2] == "out" ? out_queue : in_queue; 
    string dashes;
    for (int i = 0; i < dash_size; i++) {
        dashes += "-";
    }
    cout << dashes << "\n" << "Command: " << command << "\n" << dashes << "\n";
    if (main_queue.size() < stoi(check[3])) {
        cout << "No such frame.\n";
    }
    else {
        for (int i = 0; i < stoi(check[3]) -1; i++) {
            main_queue.pop();
        }
        string queue_type = check[2] == "out" ? "outgoing" : "incoming";
        cout << "Current Frame #" << check[3] <<  " on the " << queue_type << " queue of client " << check[1] << "\n"; 
        stack<Packet*> frame = main_queue.front();
        int hop_num = log_index != -1 ? log[log_index].number_of_hops : 0;
        PhysicalLayerPacket* p = static_cast<PhysicalLayerPacket*>(frame.top());;
        frame.pop();
        NetworkLayerPacket* n = static_cast<NetworkLayerPacket*>(frame.top());;
        frame.pop();
        TransportLayerPacket* t = static_cast<TransportLayerPacket*>(frame.top());;
        frame.pop();
        ApplicationLayerPacket* a = static_cast<ApplicationLayerPacket*>(frame.top());;
        frame.pop();
        cout << "Carried Message: " << "\"" << a->message_data << "\"\n";
        cout << "Layer 0 info: ";
        a->print();
        cout << "Layer 1 info: ";
        t->print();
        cout << "Layer 2 info: ";
        n->print();
        cout << "Layer 3 info: ";
        p->print();
        cout << "Number of hops so far: " << hop_num << "\n";
    }
}

void Network::print_log(vector<Client> &clients, vector<string> &check, string &command) {

    int dash_size = 9 + command.size();
    string dashes;
    for (int i = 0; i < dash_size; i++) {
        dashes += "-";
    }
    vector<Log> log;
    for (Client c : clients) {
        if (c.client_id == check[1]) {
            log = c.log_entries;
            break;
        }
    }

    string client_string = "Client " + check[1] + " Logs: ";
    string client_dashes;
    for (int i = 0; i < client_string.size(); i++) {
        client_dashes += "-";
    }

    cout << dashes << "\n" << "Command: " << command << "\n" << dashes << "\n";
    if (log.size() != 0) {
        cout << client_string << "\n";
    }
    for (int i = 0; i < log.size(); i++) {
        cout << client_dashes << endl;
        cout << "Log Entry #" << i + 1 << ":\n";
        cout << "Activity: " << activity_type_to_string(log[i].activity_type) << endl;
        cout << "Timestamp: " << log[i].timestamp << endl;
        cout << "Number of frames: " << log[i].number_of_frames << endl;
        cout << "Number of hops: " << log[i].number_of_hops << endl;
        cout << "Sender ID: " << log[i].sender_id << endl;
        cout << "Receiver ID: " << log[i].receiver_id << endl;
        string yes_no = log[i].success_status ? "Yes" : "No";
        cout << "Success: " << yes_no << endl;
        if (log[i].message_content.size() != 0) {
            cout << "Message: " << "\"" << log[i].message_content << "\"" << endl;
        }
    }

}

string Network::activity_type_to_string(ActivityType type) {
    switch (type) {
        case ActivityType::MESSAGE_RECEIVED:
            return "Message Received";
        case ActivityType::MESSAGE_FORWARDED:
            return "Message Forwarded";
        case ActivityType::MESSAGE_SENT:
            return "Message Sent";
        case ActivityType::MESSAGE_DROPPED:
            return "Message Dropped";
        default:
            return "";
    }
}

void Network::send(vector<Client> &clients) {
    string dashes;
    for (int i = 0; i < 13; i++) {
        dashes += "-";
    }
    cout << dashes << "\nCommand: SEND\n" << dashes << endl;
    for (Client &c : clients) {
        if (!c.outgoing_queue.empty()) {
            print_send(c, clients);
            while (!c.outgoing_queue.empty()) {
                int index = find_receiver_index(c, clients);
                stack<Packet*> frame = c.outgoing_queue.front();
                clients[index].incoming_queue.push(frame);
                c.outgoing_queue.pop();
            }
            for (Log &l : c.log_entries) {
                l.number_of_hops++;
            }
        }
    }

}

void Network::print_send(Client client, vector<Client> clients) {
    int frame_num = 1;
    while (!client.outgoing_queue.empty()) {
        stack<Packet*> frame = client.outgoing_queue.front();
        string next_hop_id = clients[find_receiver_index(client, clients)].client_id;
        PhysicalLayerPacket* p = static_cast<PhysicalLayerPacket*>(frame.top());
        frame.pop();
        NetworkLayerPacket* n = static_cast<NetworkLayerPacket*>(frame.top());
        frame.pop();
        TransportLayerPacket* t = static_cast<TransportLayerPacket*>(frame.top());
        frame.pop();
        ApplicationLayerPacket* a = static_cast<ApplicationLayerPacket*>(frame.top());
        string chunk = a->message_data;
        frame.pop();
        int dash_size = 7 + a->sender_ID.size();
        string dashes;
        for (int i = 0; i < dash_size; i++) {
            dashes += "-";
        }
        cout << "Client " << id_from_mac(clients, p->sender_MAC_address) << " sending frame #" << frame_num << " to client " << next_hop_id << endl;
        p->print();
        n->print();
        t->print();
        a->print();
        cout << "Message chunk carried: \"" << a->message_data << "\"" << endl;
        cout << "Number of hops so far: " << 1 << "\n" << dashes << endl;
        if (chunk[chunk.size()-1] == '!' || chunk[chunk.size()-1] == '.' || chunk[chunk.size()-1] == '?') {
            frame_num = 1;
        }
        else {
            frame_num++;
        }
        client.outgoing_queue.pop();
    }
}


int Network::find_receiver_index(Client client, vector<Client> clients) {
    int res;
    stack<Packet*> frame = client.outgoing_queue.front();
    PhysicalLayerPacket* p = static_cast<PhysicalLayerPacket*>(frame.top());
    for (int i = 0; i < clients.size(); i++) {
        if (clients[i].client_mac == p->receiver_MAC_address) {
            res = i;
            break;
        }
    }
    return res;
}

void Network::receive(vector<Client> &clients) {
    string dashes;
    for (int i = 0; i < 16; i++) {
        dashes += "-";
    }
    cout << dashes << "\nCommand: RECEIVE\n" << dashes << endl;
    for (Client &c : clients) {
        if (!c.incoming_queue.empty()) {
            int frame_num = 1;
            bool header = true;
            while (!c.incoming_queue.empty()) {
                if (is_client_receiver(c, c.incoming_queue.front())) {
                    string message;
                    while (true) {
                        stack<Packet*> frame = c.incoming_queue.front();
                        PhysicalLayerPacket* p = static_cast<PhysicalLayerPacket*>(frame.top());
                        frame.pop();
                        NetworkLayerPacket* n = static_cast<NetworkLayerPacket*>(frame.top());
                        frame.pop();
                        TransportLayerPacket* t = static_cast<TransportLayerPacket*>(frame.top());
                        frame.pop();
                        ApplicationLayerPacket* a = static_cast<ApplicationLayerPacket*>(frame.top());
                        frame.pop();
                        string receiver = id_from_mac(clients, p->receiver_MAC_address);
                        string sender = id_from_mac(clients, p->sender_MAC_address);
                        cout << "Client " << receiver << " receiving frame #" << frame_num << " from client " << sender << ", originating from client " << a->sender_ID << endl;
                        p->print();
                        n->print();
                        t->print();
                        a->print();
                        cout << "message chunk carried: \"" << a->message_data << "\"\n" << "Number of hops so far: 0\n";
                        string client_dash;
                        for (int i = 0; i < 7 + c.client_id.size(); i++) {
                            client_dash += "-";
                        }
                        cout << client_dash << endl;
                        frame_num++;
                        message += a->message_data;
                        if (a->message_data[a->message_data.size() - 1] == '!' || a->message_data[a->message_data.size() - 1] == '.' || a->message_data[a->message_data.size() - 1] == '?') {
                            cout << "Client " << receiver << " received the message \"" << message << "\" from client " << a->sender_ID << endl;
                            cout << client_dash << endl;
                            string timestamp = get_time_stamp();
                            ActivityType type = ActivityType::MESSAGE_RECEIVED;
                            Log log(timestamp, message, frame_num-1, 0, a->sender_ID, a->receiver_ID, true, type);
                            c.log_entries.push_back(log);
                            frame_num = 1;
                            delete p; delete t; delete n; delete a;
                            freePacketStack(frame);
                            c.incoming_queue.pop();
                            break;
                        }
                        else{
                            delete p; delete t; delete n; delete a;
                            freePacketStack(frame);
                            c.incoming_queue.pop();
                        }
                        
                    }
                }
                else {
                    string next_hop_mac = find_next_hop(c, c.incoming_queue.front(), clients);
                    if (next_hop_mac == "") {
                        stack<Packet*> frame = c.incoming_queue.front();
                        PhysicalLayerPacket* p = static_cast<PhysicalLayerPacket*>(frame.top());
                        frame.pop();
                        NetworkLayerPacket* n = static_cast<NetworkLayerPacket*>(frame.top());
                        frame.pop();
                        TransportLayerPacket* t = static_cast<TransportLayerPacket*>(frame.top());
                        frame.pop();
                        ApplicationLayerPacket* a = static_cast<ApplicationLayerPacket*>(frame.top());
                        frame.pop();
                        cout << "Client " << id_from_mac(clients, c.client_mac) << " receiving " << "frame #" << frame_num << " from Client " << a->sender_ID <<  ", but intended for Client " << a->receiver_ID << ". Forwarding...\n";
                        cout << "Error: Unreachable destination. Packets are dropped after 10 hops!\n";
                        frame_num++;
                        if (a->message_data[a->message_data.size() - 1] == '!' || a->message_data[a->message_data.size() - 1] == '.' || a->message_data[a->message_data.size() - 1] == '?') {
                            string client_dash;
                            for (int i = 0; i < 7 + c.client_id.size(); i++) {
                                client_dash += "-";
                            }
                            cout << client_dash << endl;
                            string timestamp = get_time_stamp();
                            ActivityType type = ActivityType::MESSAGE_DROPPED;
                            Log log(timestamp, "", frame_num-1, 0, a->sender_ID, a->receiver_ID, false, type);
                            c.log_entries.push_back(log);
                            frame_num = 1;
                        }
                        delete p; delete t; delete n; delete a;
                        freePacketStack(frame);
                        c.incoming_queue.pop();
                    }
                    else {
                        stack<Packet*> frame = c.incoming_queue.front();
                        PhysicalLayerPacket* p = static_cast<PhysicalLayerPacket*>(frame.top());
                        frame.pop();
                        NetworkLayerPacket* n = static_cast<NetworkLayerPacket*>(frame.top());
                        frame.pop();
                        TransportLayerPacket* t = static_cast<TransportLayerPacket*>(frame.top());
                        frame.pop();
                        ApplicationLayerPacket* a = static_cast<ApplicationLayerPacket*>(frame.top());
                        frame.pop();
                        if (header) {
                            cout << "Client " << id_from_mac(clients, c.client_mac) << " receiving a message from Client " << a->sender_ID <<  ", but intended for Client " << a->receiver_ID << ". Forwarding...\n";
                            header = false;
                        }

                        cout << "Frame #" << frame_num << " MAC address change: " << "New sender MAC " << c.client_mac << ", new receiver MAC " << next_hop_mac << endl;
                        p->sender_MAC_address = c.client_mac;
                        p->receiver_MAC_address = next_hop_mac;
                        frame.push(a);
                        frame.push(t);
                        frame.push(n);
                        frame.push(p);
                        c.outgoing_queue.push(frame);
                        c.incoming_queue.pop();
                        frame_num++;
                        if (a->message_data[a->message_data.size() - 1] == '!' || a->message_data[a->message_data.size() - 1] == '.' || a->message_data[a->message_data.size() - 1] == '?') {
                            string timestamp = get_time_stamp();
                            ActivityType type = ActivityType::MESSAGE_FORWARDED;
                            Log log(timestamp, "", frame_num-1, 0, a->sender_ID, a->receiver_ID, true, type);
                            c.log_entries.push_back(log);
                            frame_num = 1;
                            string client_dash;
                            for (int i = 0; i < 7 + c.client_id.size(); i++) {
                                client_dash += "-";
                            }
                            cout << client_dash << endl;
                            header = true;
                        }                        
                    }
                    
                }
            }
        }
    }
}

string Network::find_next_hop(Client client, stack<Packet*> frame, vector<Client> clients) {
    string res = "";
    PhysicalLayerPacket* p = static_cast<PhysicalLayerPacket*>(frame.top());
    frame.pop();
    NetworkLayerPacket* n = static_cast<NetworkLayerPacket*>(frame.top());
    frame.pop();
    TransportLayerPacket* t = static_cast<TransportLayerPacket*>(frame.top());
    frame.pop();
    ApplicationLayerPacket* a = static_cast<ApplicationLayerPacket*>(frame.top());
    frame.pop();
    for (Client c : clients) {
        if (c.client_id == a->receiver_ID) {
            res = c.client_id;
            break;
        }
    }

    if (res.size() != 0) {
        string mac = "";
        for (Client c : clients) {
            if (c.client_id == client.routing_table[res]) {
                mac = c.client_mac;
                break;
            }
        }
        return mac;
    }
    return res;
}

string Network::id_from_mac(vector<Client> clients, string mac) {
    string res = "";
    for (Client c : clients) {
        if (c.client_mac == mac) {
            res = c.client_id;
            break;
        }
    }
    return res;
} 

bool Network::is_client_receiver(Client client, stack<Packet*> frame) {
    PhysicalLayerPacket* p = static_cast<PhysicalLayerPacket*>(frame.top());
    frame.pop();
    NetworkLayerPacket* n = static_cast<NetworkLayerPacket*>(frame.top());
    frame.pop();
    TransportLayerPacket* t = static_cast<TransportLayerPacket*>(frame.top());
    frame.pop();
    ApplicationLayerPacket* a = static_cast<ApplicationLayerPacket*>(frame.top());
    frame.pop();

    return a->receiver_ID == client.client_id;
}

void Network::freePacketStack(stack<Packet*>& packetStack) {
    while (!packetStack.empty()) {
        delete packetStack.top();
        packetStack.pop();
    }
}

Network::~Network() {
    for (Client &c : client_list) {
        while (!c.incoming_queue.empty()) {
            freePacketStack(c.incoming_queue.front());
            c.incoming_queue.pop();
        }
        while (!c.outgoing_queue.empty()) {
            freePacketStack(c.outgoing_queue.front());
            c.outgoing_queue.pop();
        }
    }
}
