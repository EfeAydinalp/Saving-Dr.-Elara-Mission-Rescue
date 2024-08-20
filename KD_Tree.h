#ifndef KD_TREE_H
#define KD_TREE_H

#include "KDT_Node.h"
#include "kNN_Data.h"

#include <vector>
#include <iostream>
#include <string>
#include <queue>

class KD_Tree {
private:
    KDTreeNode* root;
    double split_threshold; // determines when to stop splitting, ie, stop growing the tree
    
public:
    
    KD_Tree(); // default constructor, sets the split_threshold to 0.1
    KD_Tree(double split_threshold); // parameterized constructor - split_threshold
    ~KD_Tree();
    KDTreeNode* buildTreeRecursive(std::vector<Point>& points, int depth);
    void build(Dataset& data);
    KDTreeNode* getRoot();


    void kNearestNeighborsUtil(KDTreeNode* node, const Point& queryPoint, int k,
                                        std::priority_queue<std::pair<double, Point>>& neighbors);

    std::vector<Point> kNearestNeighbors(const Point& queryPoint, int k);

    double calculateDistance(const Point& p1, const Point& p2);
    bool belowThreshold(const std::vector<Point>& points, double threshold);
    double calculateVariance(const std::vector<Point>& points, int dimension);


};

#endif // KD_TREE_H