#include <algorithm>
#include "KD_Tree.h"
#include <cmath> // for sqrt and pow
#include <limits> // for numeric_limits
#include <queue>
#include <string>
/*
kd_tree_leaf_node::kd_tree_leaf_node(Dataset dataLabelPairs, std::string basicString) {
    // Initialize the leaf node's data using the provided Dataset
    this->data = dataLabelPairs; // Assuming 'data' is a member variable representing Dataset in kd_tree_leaf_node

    // Other initialization if needed for 'basicString' or other members
}

kd_tree_leaf_node::~kd_tree_leaf_node() {
    // Destructor logic, if needed
}
*/
// Default constructor implementation
KD_Tree::KD_Tree() : root(nullptr), split_threshold(0.1) {
}

// Parameterized constructor implementation
KD_Tree::KD_Tree(double threshold) : root(nullptr), split_threshold(threshold) {
}

// Destructor implementation
KD_Tree::~KD_Tree() {
    // Implementation for safely deleting the KD_Tree and its nodes
}


void KD_Tree::build(Dataset& data) {
    // Implementation for building the KD_Tree using the provided dataset
    // Build the KD-Tree recursively
    root = buildTreeRecursive(data.points, 0);
}

// Function to calculate the variance along a dimension
double KD_Tree::calculateVariance(const std::vector<Point>& points, int dimension) {
    if (points.size() <= 1) {
        return 0.0;
    }

    double sum = 0.0;
    double squareSum = 0.0;

    for (const Point& point : points) {
        double val = point.features[dimension];
        sum += val;
        squareSum += val * val;
    }

    double mean = sum / points.size();
    double variance = (squareSum / points.size()) - (mean * mean);
    return variance;
}
// Function to check if the variance is below a predefined threshold
bool KD_Tree::belowThreshold(const std::vector<Point>& points, double threshold) {
    int dimensions = points[0].features.size();
    for (int i = 0; i < dimensions; ++i) {
        double variance = calculateVariance(points, i);
        if (variance > threshold) {
            return false;
        }
    }
    return true;
}

double KD_Tree::calculateDistance(const Point& p1, const Point& p2) {
    double distance = 0.0;
    int dimensions = p1.features.size();

    for (int i = 0; i < dimensions; ++i) {
        distance += pow(p1.features[i] - p2.features[i], 2);
    }

    return sqrt(distance);
}

KDTreeNode* KD_Tree::buildTreeRecursive(std::vector<Point>& points, int depth) {
    if (points.empty()) {
        return nullptr;
    }

    int dimensions = points[0].features.size();
    int axis = depth % dimensions;

    if (belowThreshold(points, split_threshold)) {
        // Create a leaf node and store the data
        Dataset dataset; // Your Dataset object initialization
        // Call the constructor with Dataset argument
        return new kd_tree_leaf_node(dataset);
    }

    // Sort points based on the current axis
    std::sort(points.begin(), points.end(),
              [axis](const Point& a, const Point& b) {
                  return a.features[axis] < b.features[axis];
              });

    size_t medianIndex = points.size() / 2;
    Point medianPoint = points[medianIndex];

    KDTreeNode* node = nullptr;

    if (points.size() > 1) {
        // Split the points into left and right subtrees
        std::vector<Point> leftPoints(points.begin(), points.begin() + medianIndex);
        std::vector<Point> rightPoints(points.begin() + medianIndex + 1, points.end());

        // Recursively build left and right subtrees
        KDTreeNode* leftChild = buildTreeRecursive(leftPoints, depth + 1);
        KDTreeNode* rightChild = buildTreeRecursive(rightPoints, depth + 1);

        // Create an internal node to store splitting information
        kd_tree_inter_node* interNode = new kd_tree_inter_node(axis, medianPoint.features[axis]);
        interNode->left = leftChild;
        interNode->right = rightChild;

        node = interNode;
    }

    return node;
}



// k-Nearest Neighbors search
std::vector<Point> KD_Tree::kNearestNeighbors(const Point& queryPoint, int k) {
    std::priority_queue<std::pair<double, Point>> neighbors;

    kNearestNeighborsUtil(root, queryPoint, k, neighbors);

    std::vector<Point> result;
    while (!neighbors.empty()) {
        result.push_back(neighbors.top().second);
        neighbors.pop();
    }

    return result;
}
void KD_Tree::kNearestNeighborsUtil(KDTreeNode* node, const Point& queryPoint, int k,
                                    std::priority_queue<std::pair<double, Point>>& neighbors) {
    if (node == nullptr) {
        return;
    }

    if (node->isLeaf()) {
        // Calculate distance from queryPoint to each point in the leaf node's data bucket
        kd_tree_leaf_node* leafNode = dynamic_cast<kd_tree_leaf_node*>(node);
        for (const Point& dataPoint : leafNode->data.points) {
            double distance = calculateDistance(dataPoint, queryPoint);
            neighbors.push(std::make_pair(distance, dataPoint));

            if (neighbors.size() > k) {
                neighbors.pop(); // Keep only k nearest neighbors in the priority queue
            }
        }
        return;
    }

    // Traverse down the tree to find the leaf node that the query point falls into
    kd_tree_inter_node* interNode = dynamic_cast<kd_tree_inter_node*>(node);
    int axis = interNode->split_dimension;
    double splitValue = interNode->split_value;

    double distance = pow(queryPoint.features[axis] - splitValue, 2);
    bool exploreLeft = queryPoint.features[axis] <= splitValue;

    kNearestNeighborsUtil(exploreLeft ? interNode->left : interNode->right, queryPoint, k, neighbors);

    if (neighbors.size() < k || distance < neighbors.top().first) {
        kNearestNeighborsUtil(exploreLeft ? interNode->right : interNode->left, queryPoint, k, neighbors);
    }
}




KDTreeNode* KD_Tree::getRoot() {
    // Implementation to return the root of the KD_Tree
    return root;
}

