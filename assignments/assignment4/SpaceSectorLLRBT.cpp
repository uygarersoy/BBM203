#include "SpaceSectorLLRBT.h"
#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    SpaceSectorBST space;
    vector<string> lines = space.read_file(filename);
    for (int i = 1; i < lines.size(); i++) {
        vector<string> values = space.split(lines[i], ',');
        insertSectorByCoordinates(stoi(values[0]), stoi(values[1]), stoi(values[2]));

    }
    root->parent = nullptr;
}

SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    delete_tree(root);
}

void SpaceSectorLLRBT::delete_tree(Sector *node) {
    if (node) {
        delete_tree(node->left);
        delete_tree(node->right);
        delete node;
    }

}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    Sector* sector = new Sector(x, y, z);
    root = insert(root, sector);
    root->color = false;
    update_parents(root);
}

void SpaceSectorLLRBT::update_parents(Sector* root) {
    if (root != nullptr) {
        if (root->left != nullptr) {
            root->left->parent = root;
            update_parents(root->left);
        }
        if (root->right != nullptr) {
            root->right->parent = root;
            update_parents(root->right);
        }
    }
}

Sector* SpaceSectorLLRBT::insert(Sector* root, Sector* sector) {
    if (root == nullptr) {
        return sector;
    }

    if (*sector < *root) {
        root->left = insert(root->left, sector);
    }
    else if (*sector > *root) {
        root->right = insert(root->right, sector);
    }
    else {
        return root;
    }

    if (is_red(root->right) && !is_red(root->left)) {
        root = rotate_left(root);
        flip_colors(root, root->left);
    }

    if (is_red(root->left) && is_red(root->left->left)) {
        root = rotate_right(root);
        flip_colors(root, root->right);
    }

    if (is_red(root->left) && is_red(root->right)) {
        root->color = !root->color;
        root->left->color = false;
        root->right->color = false;
    }
    return root;
}

bool SpaceSectorLLRBT::is_red(Sector* sector) {
    return sector != nullptr && sector->color;
}

void SpaceSectorLLRBT::flip_colors(Sector* sector, Sector* other) {
    bool temp = sector->color;
    sector->color = other->color;
    other->color = temp;
}

Sector* SpaceSectorLLRBT::rotate_left(Sector* sector) {
    Sector* right = sector->right;
    Sector* right_left = right->left;
    right->left = sector;
    sector->right = right_left;
    return right;
}

Sector* SpaceSectorLLRBT::rotate_right(Sector* sector) {
    Sector* left = sector->left;
    Sector* left_right = left->right;
    left->right = sector;
    sector->left = left_right;
    return left;
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    cout << "Space sectors inorder traversal:\n";
    in_order(root);
    cout << "\n";
}

void SpaceSectorLLRBT::in_order(Sector* root) {
    if (root != nullptr) {
        in_order(root->left);
        cout << (root->color ? "RED" : "BLACK") << " sector: " << root->sector_code << endl;
        in_order(root->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    cout << "Space sectors preorder traversal:\n";
    pre_order(root);
    cout << "\n";
}

void SpaceSectorLLRBT::pre_order(Sector* root) {
    if (root != nullptr) {
        cout << (root->color ? "RED" : "BLACK") << " sector: " << root->sector_code << endl;
        pre_order(root->left);
        pre_order(root->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    cout << "Space sectors postorder traversal:\n";
    post_order(root);
    cout << "\n";
}

void SpaceSectorLLRBT::post_order(Sector* root) {
    if (root != nullptr) {
        post_order(root->left);
        post_order(root->right);
        cout << (root->color ? "RED" : "BLACK") << " sector: " << root->sector_code << endl;
    }
}


vector<Sector*> SpaceSectorLLRBT::getStellarPath(const string& sector_code) {
    vector<Sector*> path;
    SpaceSectorBST space;
    Sector* earth = space.find_sector(root, "0SSS");
    Sector* elara = space.find_sector(root, sector_code);
    if (elara == nullptr) {
        return path;
    }
    vector<Sector*> earth_to_root = from_node_to_root(earth);
    vector<Sector*> elara_to_root = from_node_to_root(elara);

    Sector* intersection;
    int index = -1;

    for (Sector*& sector : earth_to_root) {
        path.push_back(sector);
        bool match = false;
        for (int i = elara_to_root.size() - 1; i >= 0; i--) {
            if (elara_to_root[i] == sector) {
                match = true;
                index = i;
                break;
            }
        }
        if (match) {
            break;
        }
    }
    if (index == -1) {
        return {};
    }
    for (int i = index - 1; i >= 0; i--) {
        path.push_back(elara_to_root[i]);
    }

    return path;
}

void SpaceSectorLLRBT::printStellarPath(const vector<Sector*>& path) {
    if (path.size() == 0) {
        cout << "A path to Dr. Elara could not be found.\n";
    }
    else {
        cout << "The stellar path to Dr. Elara: ";
        for (int i = 0; i < path.size()-1; i++) {
            cout << path[i]->sector_code << "->";
        }
        cout << path[path.size()-1]->sector_code << endl;
    }
}

vector<Sector*> SpaceSectorLLRBT::from_node_to_root(Sector* sector) {
    vector<Sector*> res;

    while (sector != nullptr) {
        res.push_back(sector);
        sector = sector->parent;
    }
    return res;
}