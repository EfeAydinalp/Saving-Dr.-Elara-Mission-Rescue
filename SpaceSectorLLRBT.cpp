#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {

}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the LLRBT sector map
    // according to the given comparison critera based on the sector coordinates.
    std::ifstream file(filename);

    if (file.is_open()) {
        std::string line;
        bool isFirstLine = true;

        while (std::getline(file, line)) {
            if (isFirstLine) {
                isFirstLine = false;
                continue;
            }

            std::stringstream ss(line);
            std::string token;
            int x, y, z;

            if (std::getline(ss, token, ','))
                x = std::stoi(token);

            if (std::getline(ss, token, ','))
                y = std::stoi(token);

            if (std::getline(ss, token, ','))
                z = std::stoi(token);

            insertSectorByCoordinates(x, y, z);
        }
        file.close();
    } else {
        std::cout << "File could not be opened!" << std::endl;
    }
}


// Remember to handle memory deallocation properly in the destructor.
SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    // TODO: Free any dynamically allocated memory in this class.

    free(root);

}

void SpaceSectorLLRBT::free(Sector* node){
    if(node != nullptr){
        free(node->left);
        free(node->right);
        delete node;
    }
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    // TODO: Instantiate and insert a new sector into the space sector LLRBT map 
    // according to the coordinates-based comparison criteria.

    root = insertHelper(root, x, y, z);
    root->color = BLACK; // Ensure root is BLACK after insertion
    setParentPointers(root, nullptr);

}

Sector* SpaceSectorLLRBT::insertHelper(Sector* node, int x, int y, int z) {
    if (node == nullptr) {
        return new Sector(x, y, z);

    }

    // Perform standard BST insertion
    if (x < node->x || (x == node->x && y < node->y) || (x == node->x && y == node->y && z < node->z)) {
        node->left = insertHelper(node->left, x, y, z);



    } else {
        node->right = insertHelper(node->right, x, y, z);


    }




    node = fixRedRedViolation(node);
    node = optimizeToLLRBT(node);


    // Ensure the root is always black

    return node;
}

std::string SpaceSectorLLRBT::color(Sector* node){
    if(node != nullptr){
        if(node->color == 1){
            return "RED";
        }
        else if(node->color == 0){
            return "BLACK";
        }
    }
    return "BLACK"; // öylesine koydum bi kontrol et
}
bool SpaceSectorLLRBT::isRed(Sector* node) {
    if (node == nullptr) {
        return false; // Null nodes are considered black
    }
    return node->color == RED;
}

Sector* SpaceSectorLLRBT::rotateLeft(Sector* node) {
    Sector* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    temp->color = node->color;
    node->color = RED;

    if (node->right != nullptr) {
        node->right->color = BLACK;
    }

    return temp;
}

Sector* SpaceSectorLLRBT::rotateRight(Sector* node) {
    Sector* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    temp->color = node->color;
    node->color = RED;

    if (node->left != nullptr) {
        node->left->color = BLACK;
    }

    return temp;
}
// Swap colors of the given node and its children
void SpaceSectorLLRBT::colorFlip(Sector* node) {
    node->color = !node->color; // Flip the color of the current node
    if (node->left != nullptr) {
        node->left->color = !node->left->color;
    }
    if (node->right != nullptr) {
        node->right->color = !node->right->color;
    }
}

Sector* SpaceSectorLLRBT::fixRedRedViolation(Sector* node) {
    if (isRed(node->right)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        colorFlip(node);
    }

    return node;
}

Sector* SpaceSectorLLRBT::optimizeToLLRBT(Sector* node) {
    // Fix violations and optimize the LLRBT
    if (isRed(node->right)) {
        node = rotateLeft(node);
    }

    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }

    if (isRed(node->left) && isRed(node->right)) {
        colorFlip(node);
    }

    return node;
}

void SpaceSectorLLRBT::setParentPointers(Sector* node, Sector* parent) {
    if (node == nullptr) {
        return;
    }

    node->parent = parent; // Set the parent pointer for the current node

    // Recursively set parent pointers for left and right subtrees
    setParentPointers(node->left, node);
    setParentPointers(node->right, node);
}


std::string SpaceSectorLLRBT::generateSectorCode(const Sector* sector) const {
    // Calculate distance component (floor of the distance)


    int distance_component = static_cast<int>(floor(sector->distance_from_earth));

    // Initialize sector code with distance component
    std::string sector_code = std::to_string(distance_component);

    // Append coordinate components based on X, Y, Z values
    sector_code += getCodeForCoordinate(sector->x,"x");
    sector_code += getCodeForCoordinate(sector->y,"y");
    sector_code += getCodeForCoordinate(sector->z,"z");

    return sector_code;
}

// Helper function to generate code for a single coordinate
std::string SpaceSectorLLRBT::getCodeForCoordinate(int value, std::string variable) const {
    if (value == 0) {
        return "S"; // Same as Earth's coordinate
    } else if (value > 0) {
        if (variable == "x") {
            return "R"; // Right (X)
        } else if (variable == "y") {
            return "U"; // Up (Y)
        } else if (variable == "z") {
            return "F"; // Forward (Z)
        }

    } else {
        if (variable == "x") {
            return "L"; // Left (X)
        } else if (variable == "y") {
            return "D"; // Down (Y)
        } else if (variable == "z") {
            return "B"; // Backward (Z)
        }

    }
    return ""; // Default case
}


void SpaceSectorLLRBT::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors
    // to STDOUT in the given format.
    std::cout << "Space sectors inorder traversal:" << std::endl;
    inOrderTraversal(root);
    cout << endl;
}

void SpaceSectorLLRBT::inOrderTraversal(Sector* node) {
    if (node != nullptr) {
        // Sol alt ağacı travers et
        inOrderTraversal(node->left);

        // Geçerli düğümün değerini yazdır

        //cout << "(" << node->x << ", " << node->y << ", " << node->z << ")" << std::endl;
        std::cout << color(node) << " sector: " << generateSectorCode(node) << std::endl;

        // Sağ alt ağacı travers et
        inOrderTraversal(node->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors preorder traversal:" << std::endl;
    preOrderTraversal(root);
    cout << endl;
}

void SpaceSectorLLRBT::preOrderTraversal(Sector* node) {
    if (node != nullptr) {
        // Geçerli düğümün değerini yazdır

        //cout << "(" << node->x << ", " << node->y << ", " << node->z << ")" << std::endl;
        std::cout << color(node) << " sector: " << generateSectorCode(node) << std::endl;

        // Sol alt ağacı travers et
        preOrderTraversal(node->left);

        // Sağ alt ağacı travers et
        preOrderTraversal(node->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors postorder traversal:" << std::endl;
    postOrderTraversal(root);
    cout << endl;
}

void SpaceSectorLLRBT::postOrderTraversal(Sector* node) {
    if (node != nullptr) {
        // Sol alt ağacı travers et
        postOrderTraversal(node->left);

        // Sağ alt ağacı travers et
        postOrderTraversal(node->right);

        // Geçerli düğümün değerini yazdır

        //cout << "(" << node->x << ", " << node->y << ", " << node->z << ")" << std::endl;
        std::cout << color(node) << " sector: " << generateSectorCode(node) << std::endl;
    }
}

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code){
    std::vector<Sector*> path;
    std::vector<Sector*> pointerVector;
    std::vector<Sector*> earthVector;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    Sector* pointer;
    Sector* earth;

    //cout << "1" << endl;

    stellarPathHelper(root, sector_code, pointerVector);
    stellarPathHelper(root, "0SSS", earthVector);
    //cout << "2" <<endl;

    //cout << pointerVector.empty() << endl;
    //cout << pointerVector[0] << endl;
    //cout << pointerVector[0]->y << endl;
    //cout << pointerVector[0]->z << endl;

    //cout << earthVector[0]->x << endl;
    //cout << earthVector[0]->y << endl;
    //cout << earthVector[0]->z << endl;

    if(!pointerVector.empty() && generateSectorCode(pointerVector[0]) == sector_code
            && !earthVector.empty() && generateSectorCode(earthVector[0]) == "0SSS"
                && pointerVector[0] != nullptr && earthVector[0] != nullptr){

        //cout << "test" << endl;

        /*
        cout << endl << pointerVector[0]->x << endl;
        cout << pointerVector[pointerVector.size()-1]->x << endl;
        cout << pointer->x << endl << endl;

        cout << pointerVector[0]->y << endl;
        cout << pointerVector[0]->z << endl << endl;

        cout << generateSectorCode(pointerVector[0])<< endl;
        cout << generateSectorCode(pointer) <<endl << endl;

        cout << sector_code.substr(sector_code.size()-3,1) << endl;
        */
/*
        cout << generateSectorCode(earth) <<endl << endl;
        cout << earth->x << endl;
        cout << earth->y << endl;
        cout << earth->z << endl;

        cout << generateSectorCode(pointer) <<endl << endl;
        cout << pointer->x << endl;
        cout << pointer->y << endl;
        cout << pointer->z << endl;
*/
        /*
        cout << "-----------------" << endl;
        cout << earth->parent->x << endl;
        cout << earth->parent->y << endl;
        cout << earth->parent->z << endl;
        cout << "-----------------" << endl;
         */
/*
        cout << generateSectorCode(root) << endl;
        cout << " ------------------------------" << endl ;
        cout << earth->parent->parent->x << endl;
        cout << earth->parent->parent->y << endl;
        cout << earth->parent->parent->z << endl;
        cout << generateSectorCode(earth->parent->parent) << endl;
*/
        pointer = pointerVector[0]; // 0 da olur test et.
        earth = earthVector[0]; // 0 da olur test et. belki if içine bunun da null olmadığını test ettirmen gerekebilir

        path.push_back(earth);

        pointerVector.clear();
        earthVector.clear();

        pathFinder(root, pointer,pointerVector); //OSSS -> sectorCode
        pathFinder(root, earth,earthVector);

        int commonPointersCount = 0;

        for (const auto& ptr1 : pointerVector) {
            for (const auto& ptr2 : earthVector) {
                if (ptr1 == ptr2) {
                    commonPointersCount++;
                    break;  // Break the inner loop if a common pointer is found
                }
            }
        }
        pointerVector.erase(pointerVector.begin(), pointerVector.begin() + commonPointersCount);
        if(commonPointersCount >= 1) {
            earthVector.erase(earthVector.begin(), earthVector.begin() + (commonPointersCount - 1));
        }
        if(commonPointersCount < 1) {
            earthVector.erase(earthVector.begin(), earthVector.begin() + (commonPointersCount));
        }
        reverse(earthVector.begin(), earthVector.end());

        for (const auto& element : earthVector) {
            path.push_back(element);
        }

        // Adding elements of vector2 to combinedVector
        for (const auto& element : pointerVector) {
            path.push_back(element);
        }

        path.push_back(pointer);
        //path.push_back(pointer);
    }
    //cout << path[2]->x;

    return path;
}


void SpaceSectorLLRBT::stellarPathHelper(Sector* node, const std::string& sector_code,
                                       std::vector<Sector*>& path) {

    if (node != nullptr) {
        nodeCheck(node ,sector_code, path);
    }

}
void SpaceSectorLLRBT::nodeCheck(Sector* node,const std::string& sector_code , std::vector<Sector*>& path) {
    if (node != nullptr) {

        if(generateSectorCode(node) == sector_code){
            path.push_back(node);
            return;

        }

        // Sol alt ağacı travers et
        nodeCheck(node->left, sector_code, path);

        // Sağ alt ağacı travers et
        nodeCheck(node->right, sector_code, path);


    }
}

void SpaceSectorLLRBT::pathFinder(Sector* node, Sector* pointer , std::vector<Sector*>& path) {

    while(node != pointer && node != nullptr){

        path.push_back(node);
        if(pointer->x > node->x){
            //cout << "Pointer " <<pointer->x <<endl;
            //cout << "Node " <<node->x<<endl;
            //cout << "Saga"<< endl;
            return pathFinder(node->right,pointer,path);
            //cout << " Saga Kaydirdim"<< endl;
        }
        else if(pointer->x < node->x){
            //cout <<"Pointer " <<pointer->x<<endl;
            //cout << "Node " <<node->x<<endl;
            //cout << "Sola"<< endl;
            return pathFinder(node->left,pointer,path);
            //cout << "Sola Kaydirdim" << endl;
        }
        else{
            if(pointer->y > node->y){
                //cout << "B";
                return pathFinder(node->right,pointer,path);
            }
            else if(pointer->y < node->y){

                return pathFinder(node->left,pointer,path);
            }
            else if (pointer->y == node->y){
                if(pointer->z > node->z){

                    return pathFinder(node->right,pointer,path);
                }
                else if(pointer->z < node->z){

                    return pathFinder(node->left,pointer,path);
                }
                else{

                    return;
                }
            }

        }
    }
}



void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function
    // to STDOUT in the given format.

    if(!path.empty()){
        cout << "The stellar path to Dr. Elara: ";
        for(int i = 0; i < path.size(); i++){
            cout << generateSectorCode(path[i]) << (i < path.size() - 1 ? "->" : "");
        }
        cout << endl;
    }
    else{
        cout << "A path to Dr. Elara could not be found." << endl;


    }

}

/*

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!
    return path;
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
    // TODO: Print the stellar path obtained from the getStellarPath() function 
    // to STDOUT in the given format.
}
*/