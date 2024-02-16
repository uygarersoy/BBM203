#ifndef SECTOR_H
#define SECTOR_H

#include <string>
#include <cmath>

const bool RED = true;
const bool BLACK = false;

class Sector {
public:

    Sector(int x, int y, int z);
    ~Sector();

    int x, y, z; 
    double distance_from_earth; 
    std::string sector_code;
    Sector *left, *right, *parent;
    bool color;

    Sector& operator=(const Sector& other);
    bool operator==(const Sector& other) const;
    bool operator!=(const Sector& other) const;

    bool operator>(const Sector& other) const;
    bool operator<(const Sector& other) const;
    double distance(int x, int y, int z);
    std::string sector_code_f(int x, int y, int z);

};

#endif // SECTOR_H