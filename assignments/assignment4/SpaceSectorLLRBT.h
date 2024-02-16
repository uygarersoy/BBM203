#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    Sector* insert(Sector* root, Sector* sector);
    bool is_red(Sector* sector);
    void flip_colors(Sector* sector, Sector* other);
    Sector* rotate_left(Sector* sector);
    Sector* rotate_right(Sector* sector);
    void update_parents(Sector* root);
    void print_child_parent(Sector* root);
    std::vector<Sector*> from_node_to_root(Sector* sector);


    void in_order(Sector* root);
    void pre_order(Sector* root);
    void post_order(Sector* root);
    void delete_tree(Sector *node);


};

#endif // SPACESECTORLLRBT_H