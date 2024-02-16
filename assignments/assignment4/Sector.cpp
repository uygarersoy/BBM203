#include "Sector.h"


Sector::Sector(int x, int y, int z) : x(x), y(y), z(z), left(nullptr), right(nullptr), parent(nullptr), color(RED) {
    distance_from_earth = distance(x, y, z);
    sector_code = std::to_string(static_cast<int>(distance_from_earth)) + sector_code_f(x, y, z);
}

Sector::~Sector() {
}

Sector& Sector::operator=(const Sector& other) {
    if (this != &other) { 
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
        this->distance_from_earth = other.distance_from_earth;
        this->sector_code = other.sector_code;
        this->left = other.left;
        this->right = other.right;
        this->parent = other.parent;
        this->color = other.color;
    }
    return *this;
}

bool Sector::operator<(const Sector& other) const{
    if (x < other.x) {
        return true;
    }
    else if (x == other.x) {
        if (y < other.y) {
            return true;
        }
        else if (y == other.y) {
            return z < other.z;
        }
    }
    return false;

}

bool Sector::operator>(const Sector& other) const{
    return other < *this;
}

bool Sector::operator==(const Sector& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

bool Sector::operator!=(const Sector& other) const {
    return !(*this == other);
}

double Sector::distance(int x, int y, int z) {
    return static_cast<double>(sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
}

std::string Sector::sector_code_f(int x, int y, int z) {
    std::string code;
    int base[3] = {x, y, z};
    std::string bigger = "RUF";
    std::string smaller = "LDB";

    for (int i = 0; i < 3; i++) {
        if (base[i] == 0) {
            code += "S";
        }
        else if (base[i] < 0) {
            code += smaller[i];
        }
        else {
            code += bigger[i];
        }
    }
    return code;
}