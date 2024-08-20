// kNN.h
#ifndef KNN_H
#define KNN_H

#include "KD_Tree.h"
#include "kNN_Data.h"
#include <vector>
#include <queue>

class KNN {
private:


public:
    KD_Tree tree;
    int k; // Number of neighbors for kNN
    double split_threshold; // Threshold for the kd_tree

    KNN(int k, double threshold);
    void train(Dataset& data); // Need to initialize the tree here 
    int predict(const Point& queryPoint);

    void kNearestNeighborsUtil(KDTreeNode *node, const Point &queryPoint,
                                    std::priority_queue<std::pair<double, Point>> &neighbors);
    std::priority_queue<Point> nearestNeighbors;
};

#endif // KNN_H