#include <cmath>
#include "SpaceSectorBST.h"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    // Free any dynamically allocated memory in this class.
    free(root);
}

void SpaceSectorBST::free(Sector* node){
    if(node != nullptr){
        free(node->left);
        free(node->right);
        delete node;
    }
}
void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    // TODO: read the sectors from the input file and insert them into the BST sector map
    // according to the given comparison critera based on the sector coordinates.
    std::ifstream file(filename); // Dosyayı okumak için bir ifstream nesnesi oluşturdum.

    if (file.is_open()) { // Dosya başarıyla açıldıysa devam et
        std::string line;
        bool isFirstLine = true; // İlk satırı atlamak için bir bool değer

        while (std::getline(file, line)) { // Dosyadan satır satır oku
            if (isFirstLine) {
                isFirstLine = false;
                continue; // İlk satırı atla
            }

            std::stringstream ss(line);
            std::string token;
            int x, y, z;

            // Virgülle ayrılmış değerleri oku ve parçala
            if (std::getline(ss, token, ','))
                x = std::stoi(token); // X değerini al

            if (std::getline(ss, token, ','))
                y = std::stoi(token); // Y değerini al

            if (std::getline(ss, token, ','))
                z = std::stoi(token); // Z değerini al

            // Burada alınan x, y, z değerlerini BST'ye ekleme işlemi yapıyor
            insertSectorByCoordinates(x, y, z);

        }
        file.close(); // Dosyayı kapat
    } else {
        std::cout << "Dosya açılamadı!" << std::endl;
    }
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    // Instantiate and insert a new sector into the space sector BST map according to the 
    // coordinates-based comparison criteria.
    root = insertHelper(root, x, y, z);
    //setParentPointers(root, nullptr);
    setParentPointers(root, nullptr);
}

// Yardımcı özyinelemeli fonksiyon
Sector* SpaceSectorBST::insertHelper(Sector* node, int x, int y, int z) {
    // Eğer düğüm boşsa, yeni bir düğüm oluşturulur ve bu düğüm geri döner
    if (node == nullptr) {

        return new Sector(x, y, z);

    }

    // X koordinatlarını karşılaştır
    if (x < node->x) {
        // Sol alt ağaca ekleme
        node->left = insertHelper(node->left, x, y, z);
    } else if (x > node->x) {
        // Sağ alt ağaca ekleme
        node->right = insertHelper(node->right, x, y, z);
    } else {
        // Eğer X koordinatları aynıysa, Y koordinatlarını karşılaştır
        if (y < node->y) {
            // Sol alt ağaca ekleme
            node->left = insertHelper(node->left, x, y, z);
        } else if (y > node->y) {
            // Sağ alt ağaca ekleme
            node->right = insertHelper(node->right, x, y, z);
        } else {
            // Eğer X ve Y koordinatları aynıysa, Z koordinatlarını karşılaştır
            if (z < node->z) {
                // Sol alt ağaca ekleme
                node->left = insertHelper(node->left, x, y, z);
            } else if (z > node->z) {
                // Sağ alt ağaca ekleme
                node->right = insertHelper(node->right, x, y, z);
            }
            // Eğer X, Y ve Z koordinatları aynıysa, aynı düğüm tekrar eklenir (duplicates kabul edilmez)
        }
    }

    return node;
}
void SpaceSectorBST::setParentPointers(Sector* node, Sector* parent) {
    if (node == nullptr) {
        return;
    }

    node->parent = parent; // Set the parent pointer for the current node

    // Recursively set parent pointers for left and right subtrees
    setParentPointers(node->left, node);
    setParentPointers(node->right, node);
}
/*
void SpaceSectorBST::setParentPointers(Sector* node, Sector* parent) {
    if (node == nullptr) {
        return;
    }

    node->parent = parent; // Set the parent pointer for the current node

    // Recursively set parent pointers for left and right subtrees
    setParentPointers(node->left, node);
    setParentPointers(node->right, node);
}
 */
void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    std::vector<Sector*> path;
    Sector* pointer;

    stellarPathHelper(root, sector_code, path);
    if(!path.empty() && generateSectorCode(path[0]) == sector_code && root != nullptr){
        pointer = path[0];
        root = deleteHelper(root, sector_code, pointer);
    }
    //root = deleteHelper(root, sector_code, nullptr);

}

Sector* SpaceSectorBST::deleteHelper(Sector* node, const std::string& sector_code, Sector* pointer) {
    if (node == nullptr) {
        return nullptr;
    }

    // If the node matches the pointer, perform deletion logic
    if (node == pointer) {
        // Case 1: Node with no children or only one child
        if (node->left == nullptr) {
            Sector* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr) {
            Sector* temp = node->left;
            delete node;
            return temp;
        }

        // Case 2: Node with two children
        Sector* successor = findSuccessor(node->right);
        node->x = successor->x;
        node->y = successor->y;
        node->z = successor->z;
        node->right = deleteHelper(node->right, generateSectorCode(successor), successor);
        return node;
    }

    if (pointer->x < node->x) {
        node->left = deleteHelper(node->left, sector_code,pointer);
    } else if (pointer->x > node->x) {
        node->right = deleteHelper(node->right, sector_code,pointer);
    } else {
        if (pointer->y < node->y) {
            node->left = deleteHelper(node->left, sector_code, pointer);
        } else if (pointer->y > node->y) {
            node->right = deleteHelper(node->right, sector_code, pointer);
        } else {
            if (pointer->z < node->z) {
                node->left = deleteHelper(node->left, sector_code, pointer);
            } else if (pointer->z > node->z) {
                node->right = deleteHelper(node->right, sector_code, pointer);
            }

        }
    }
    return node;
}

// Helper function to find the inorder successor (leftmost node in the right subtree)
Sector* SpaceSectorBST::findSuccessor(Sector* node) {
    Sector* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}
/*
void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    // TODO: Delete the sector given by its sector_code from the BST.
    std::vector<Sector*> path;
    Sector* pointer;

    stellarPathHelper(root, sector_code, path);
    if(!path.empty() && generateSectorCode(path[0]) == sector_code && root != nullptr){
        pointer = path[0];
        root = deleteHelper(root,pointer,sector_code);
    }

}



Sector* SpaceSectorBST::deleteHelper(Sector* node, Sector* pointer, const std::string& sector_code) {
    if (node == nullptr) {
        return nullptr;
    }

    if (generateSectorCode(node) == sector_code) {
        // Case 1: Node with no children or only one child
        if (node->left == nullptr && node->right != nullptr) {
            Sector* temp = node->right;
            delete node;
            return temp;
        } else if (node->right == nullptr && node->left != nullptr) {
            Sector* temp = node->left;
            delete node;
            return temp;
        }
        else if (node->right == nullptr && node->left == nullptr) {

            delete node;
            return nullptr;
        }

        // Case 2: Node with two children
        Sector* successor = findSuccessor(node->right);
        node->x = successor->x;
        node->y = successor->y;
        node->z = successor->z;
        node->right = deleteHelper(node->right,pointer, generateSectorCode(successor));
        return node;
    }

    if(node->x < pointer->x){
        node->right = deleteHelper(node->right,pointer, sector_code);
    }
    else if(node->x > pointer->x){
        node->left = deleteHelper(node->left,pointer, sector_code);
    }
    else{
        if(node->y < pointer->y){
            node->right = deleteHelper(node->right,pointer, sector_code);
        }
        else if(node->y > pointer->y){
            node->left = deleteHelper(node->left,pointer, sector_code);
        }
        else {
            if (node->z < pointer->z) {
                node->right = deleteHelper(node->right, pointer, sector_code);
            } else if (node->z > pointer->z) {
                node->left = deleteHelper(node->left, pointer, sector_code);
            }
        }


        }

    return node;
}
// Helper function to find the inorder successor (leftmost node in the right subtree)
Sector* SpaceSectorBST::findSuccessor(Sector* node) {
    Sector* current = node;
    while (current && current->left != nullptr) {
        current = current->left;
    }
    return current;
}
*/
std::string SpaceSectorBST::generateSectorCode(const Sector* sector) const {
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
std::string SpaceSectorBST::getCodeForCoordinate(int value, std::string variable) const {
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


void SpaceSectorBST::displaySectorsInOrder() {
    // TODO: Traverse the space sector BST map in-order and print the sectors 
    // to STDOUT in the given format.
    std::cout << "Space sectors inorder traversal:" << std::endl;
    inOrderTraversal(root);
    cout << endl;
}

void SpaceSectorBST::inOrderTraversal(Sector* node) {
    if (node != nullptr) {
        // Sol alt ağacı travers et
        inOrderTraversal(node->left);

        // Geçerli düğümün değerini yazdır

        //cout << "(" << node->x << ", " << node->y << ", " << node->z << ")" << std::endl;
        std::cout << generateSectorCode(node) << std::endl;

        // Sağ alt ağacı travers et
        inOrderTraversal(node->right);
    }
}

void SpaceSectorBST::displaySectorsPreOrder() {
    // TODO: Traverse the space sector BST map in pre-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors preorder traversal:" << std::endl;
    preOrderTraversal(root);
    cout << endl;
}

void SpaceSectorBST::preOrderTraversal(Sector* node) {
    if (node != nullptr) {
        // Geçerli düğümün değerini yazdır

        //cout << "(" << node->x << ", " << node->y << ", " << node->z << ")" << std::endl;
        std::cout << generateSectorCode(node) << std::endl;

        // Sol alt ağacı travers et
        preOrderTraversal(node->left);

        // Sağ alt ağacı travers et
        preOrderTraversal(node->right);
    }
}

void SpaceSectorBST::displaySectorsPostOrder() {
    // TODO: Traverse the space sector BST map in post-order traversal and print 
    // the sectors to STDOUT in the given format.
    std::cout << "Space sectors postorder traversal:" << std::endl;
    postOrderTraversal(root);
    cout << endl;
}

void SpaceSectorBST::postOrderTraversal(Sector* node) {
    if (node != nullptr) {
        // Sol alt ağacı travers et
        postOrderTraversal(node->left);

        // Sağ alt ağacı travers et
        postOrderTraversal(node->right);

        // Geçerli düğümün değerini yazdır

        //cout << "(" << node->x << ", " << node->y << ", " << node->z << ")" << std::endl;
        std::cout << generateSectorCode(node) << std::endl;
    }
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    std::vector<Sector*> pointerVector;

    // TODO: Find the path from the Earth to the destination sector given by its
    // sector_code, and return a vector of pointers to the Sector nodes that are on
    // the path. Make sure that there are no duplicate Sector nodes in the path!

    Sector* pointer;

    stellarPathHelper(root, sector_code, pointerVector);
    if(!pointerVector.empty() && generateSectorCode(pointerVector[0]) == sector_code && root != nullptr){

        pointer = pointerVector[pointerVector.size()-1]; // 0 da olur test et.

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
        pathFinder(root,pointer,path);
        path.push_back(pointer);
    }
    //cout << path[2]->x;

    return path;
}


void SpaceSectorBST::stellarPathHelper(Sector* node, const std::string& sector_code,
                                std::vector<Sector*>& path) {

    if (node != nullptr) {

        if(sector_code.substr(sector_code.size() - 3, 1) == "L"){

             nodeCheck(node->left ,sector_code, path);

        }
        else if(sector_code.substr(sector_code.size() - 3, 1) == "R"){

             nodeCheck(node->right,sector_code, path);

        }
        else if(sector_code.substr(sector_code.size() - 3, 1) == "S"){
            nodeCheck(node ,sector_code, path);
        }
    }
}
void SpaceSectorBST::nodeCheck(Sector* node,const std::string& sector_code , std::vector<Sector*>& path) {
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

void SpaceSectorBST::pathFinder(Sector* node,Sector* pointer , std::vector<Sector*>& path) {

    while(generateSectorCode(node) != generateSectorCode(pointer)){

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



void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
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