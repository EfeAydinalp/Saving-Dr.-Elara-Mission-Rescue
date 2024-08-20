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

    Sector* insertHelper(Sector* node, int x, int y, int z);
    //void setParentPointers(Sector* node, Sector* parent);
    void inOrderTraversal(Sector* node);
    void preOrderTraversal(Sector* node);
    void postOrderTraversal(Sector* node);
    Sector* deleteHelper(Sector* node, const std::string& sector_code, Sector* pointer);
    //Sector* deleteHelper(Sector* node,Sector* pointer, const std::string& sector_code);
    Sector* findSuccessor(Sector* node);
    //Sector* searchSector(Sector* node, const std::string& sector_code);
    //void findPathToSector(Sector* node, Sector* destination, std::vector<Sector*>& path);
    std::string generateSectorCode(const Sector* sector) const;
    std::string getCodeForCoordinate(int value, std::string variable) const;
    void stellarPathHelper(Sector* node, const std::string& sector_code, std::vector<Sector*>& path);
    void nodeCheck(Sector* node,const std::string& sector_code, std::vector<Sector*>& path);
    void pathFinder(Sector* node, Sector* pointer, std::vector<Sector*>& path);
    void setParentPointers(Sector* node, Sector* parent);
    void free(Sector* node);
};

#endif // SPACESECTORBST_H
