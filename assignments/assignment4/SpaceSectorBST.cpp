#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    delete_tree(root);
}

void SpaceSectorBST::delete_tree(Sector *node) {
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }

}

void SpaceSectorBST::readSectorsFromFile(const string& filename) {
    vector<string> lines = read_file(filename);
    for (int i = 1; i < lines.size(); i++) {
        vector<string> values = split(lines[i], ',');
        insertSectorByCoordinates(stoi(values[0]), stoi(values[1]), stoi(values[2]));
    }

}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    Sector* sector = new Sector(x, y, z);
    root = insert(root, sector);
}

Sector* SpaceSectorBST::insert(Sector* sector, Sector* new_sector) {
    if (sector == nullptr) {
        return new_sector;
    }
    if (*new_sector < *sector) {
        sector->left = insert(sector->left, new_sector);
    }
    else if (*new_sector > *sector) {
        sector->right = insert(sector->right, new_sector);
    }
    return sector;
}

void SpaceSectorBST::deleteSector(const string& sector_code) {
    Sector* sector = find_sector(root, sector_code);
    _delete(sector);
}

Sector* SpaceSectorBST::find_sector(Sector* sector, string sector_code) {
    if (sector == nullptr) {
        return nullptr;
    }
    if (sector->sector_code == sector_code) {
        return sector;
    }
    
    Sector* left_tree = find_sector(sector->left, sector_code);
    Sector* right_tree = find_sector(sector->right, sector_code);

    return (left_tree != nullptr) ? left_tree : right_tree;
}

Sector* SpaceSectorBST::find_successor(Sector* sector) {
    sector = sector->right;
    while (sector->left != nullptr) {
        sector = sector->left;
    }
    return sector;
}

void SpaceSectorBST::_delete(Sector* sector) {
    if (sector == nullptr) {
        return;
    }
    else if (sector->left == nullptr && sector->right == nullptr) {
        if (sector == root) {
            delete root;
            root = nullptr;
        }
        else {
            Sector* parent = find_parent(root, sector);
            if (parent->left == sector) {
                delete parent->left;
                parent->left = nullptr;
            }
            else {
                delete parent->right;
                parent->right = nullptr; 
            }
        }
        
    }
    else if (sector->left == nullptr && sector->right != nullptr) {
        Sector* temp = sector->right;
        *sector = *(sector->right);
        delete temp;
    }
    else if(sector->right == nullptr && sector->left != nullptr) {
        Sector* temp = sector->left;
        *sector = *(sector->left);
        delete temp;
    }
    else {
        Sector* successor = find_successor(sector);
        sector->x = successor->x;
        sector->y = successor->y;
        sector->z = successor->z;
        sector->distance_from_earth = successor->distance_from_earth;
        sector->sector_code = successor->sector_code;
        _delete(successor);
    }
}

Sector* SpaceSectorBST::find_parent(Sector* root, Sector* child) {
    if (root != nullptr) {
        if (root->left == child) {
            return root;
        }
        else if (root->right == child) {
            return root;
        } 
        Sector* left_check = find_parent(root->left, child);
        Sector* right_check = find_parent(root->right, child);

        return (left_check != nullptr) ? left_check : right_check;
    }
    return root;
}

void SpaceSectorBST::displaySectorsInOrder() {
    cout << "Space sectors inorder traversal:\n";
    in_order(root);
    cout << "\n";
}

void SpaceSectorBST::in_order(Sector* root) {
    if (root != nullptr) {
        in_order(root->left);
        cout << root->sector_code << endl;
        in_order(root->right);
    }
}


void SpaceSectorBST::displaySectorsPreOrder() {
    cout << "Space sectors preorder traversal:\n";
    pre_order(root);
    cout << "\n";
}

void SpaceSectorBST::pre_order(Sector* root) {
    if (root != nullptr) {
        cout << root->sector_code << endl;
        pre_order(root->left);
        pre_order(root->right);
    }
}

void SpaceSectorBST::displaySectorsPostOrder() {
    cout << "Space sectors postorder traversal:\n";
    post_order(root);
    cout << "\n";
}

void SpaceSectorBST::post_order(Sector* root) {
    if (root != nullptr) {
        post_order(root->left);
        post_order(root->right);
        cout << root->sector_code << endl;
    }
}

vector<Sector*> SpaceSectorBST::getStellarPath(const string& sector_code) {
    vector<Sector*> path;
    if (is_path_exist(root, path, sector_code)) {
        return path;
    }
    return {};
}

bool SpaceSectorBST::is_path_exist(Sector* root, vector<Sector*>& path, string sector_code) {
    if (root == nullptr) {
        return false;
    }
    path.push_back(root);
    if (root->sector_code == sector_code) {
        return true;
    }

    if (is_path_exist(root->left, path, sector_code) || is_path_exist(root->right, path, sector_code)) {
        return true;
    }
    path.pop_back();
    return false;
}

void SpaceSectorBST::printStellarPath(const vector<Sector*>& path) {
    if (path.size() != 0) {
        cout << "The stellar path to Dr. Elara: ";
        for (int i = 0; i < path.size()-1; i++) {
            cout << path[i]->sector_code << "->";
        }
        cout << path[path.size()-1]->sector_code << "\n";
    }
    else {
        cout <<"A path to Dr. Elara could not be found.\n";
    }
    

}

vector<string> SpaceSectorBST::read_file(const string &file_name) {
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

vector<string> SpaceSectorBST::split(string line, char delimiter) {
    vector<string> vec;
    string temp;
    stringstream ss(line);

    while (getline(ss, temp, delimiter)) {
        vec.push_back(temp);
    }
    return vec;
}