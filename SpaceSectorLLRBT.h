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


    Sector* rotateLeft(Sector* node);
    Sector* rotateRight(Sector* node);
    //Sector* insert(Sector* node, Sector* newNode);
    Sector* optimizeToLLRBT(Sector* node);
    bool isRed(Sector* node);
    void colorFlip(Sector* node);
    Sector* insertHelper(Sector* node, int x, int y, int z);
    Sector* fixRedRedViolation(Sector* node);
    void postOrderTraversal(Sector* node);
    void preOrderTraversal(Sector* node);
    void inOrderTraversal(Sector* node);
    std::string getCodeForCoordinate(int value, std::string variable) const;
    std::string generateSectorCode(const Sector* sector) const;
    static std::string color(Sector* node);
    void setParentPointers(Sector* node, Sector* parent);
    void stellarPathHelper(Sector* node, const std::string& sector_code,std::vector<Sector*>& path);
    void nodeCheck(Sector* node,const std::string& sector_code , std::vector<Sector*>& path);
    void pathFinder(Sector* node, Sector* pointer , std::vector<Sector*>& path);
    void free(Sector* node);
};

#endif // SPACESECTORLLRBT_H
