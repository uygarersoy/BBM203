#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    void deleteSector(const std::string& sector_code);
    void displaySectorsInOrder();
    void displaySectorsPreOrder();
    void displaySectorsPostOrder();
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);

    std::vector<std::string> read_file(const std::string &file_name);
    std::vector<std::string> split(std::string line, char delimiter);
    Sector* insert(Sector* sector, Sector* new_sector);
    void in_order(Sector* root);
    void pre_order(Sector* root);
    void post_order(Sector* root);
    void delete_tree(Sector *node);
    Sector* find_sector(Sector* sector, std::string sector_code);
    Sector* find_successor(Sector* sector);
    void _delete(Sector* sector);
    Sector* find_parent(Sector* root, Sector* child);
    bool is_path_exist(Sector* root, std::vector<Sector*>& path, std::string sector_code);

};

#endif // SPACESECTORBST_H