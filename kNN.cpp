#include "kNN.h"
#include <queue> // For priority queue
#include <valarray>
#include <unordered_map>

double calculateDistance(const std::vector<double>& p1, const std::vector<double>& p2) {
    // Assuming both points have the same dimensionality
    double distance = 0.0;
    for (size_t i = 0; i < p1.size(); ++i) {
        distance += (p1[i] - p2[i]) * (p1[i] - p2[i]);
    }
    return sqrt(distance);
}


// Constructor implementation
KNN::KNN(int neighbors, double threshold) : k(neighbors), split_threshold(threshold) {
}

// Train function implementation
void KNN::train(Dataset& data) {
    // standardize the data

    // build the KD_Tree
    tree.build(data);
}

// Predict function implementation
int KNN::predict(const Point& queryPoint) {
    // Returns 1 if the given queryPoint is habitable, 0 otherwise

    // Start traversal from the root
    KDTreeNode *node = tree.getRoot();
    while (!node->isLeaf()) {
        kd_tree_inter_node *interNode = dynamic_cast<kd_tree_inter_node *>(node);
        if (queryPoint.features[interNode->split_dimension] < interNode->split_value) {
            node = interNode->left;
        } else {
            node = interNode->right;
        }
    }


    // Search within the leaf node for nearest neighbors
    kd_tree_leaf_node *leafNode = dynamic_cast<kd_tree_leaf_node *>(node);
    std::priority_queue<std::pair<double, Point>> nearestNeighbors;

    for (const auto &dataPoint: leafNode->data.points) {
        double dist = calculateDistance(queryPoint.features, dataPoint.features);
        nearestNeighbors.push({dist, dataPoint});
        if (nearestNeighbors.size() > k) {
            nearestNeighbors.pop(); // Keep only 'k' nearest neighbors
        }
    }


    std::unordered_map<std::string, int> labelCounts;
    while (!nearestNeighbors.empty()) {
        const Point &neighbor = nearestNeighbors.top().second;
        labelCounts[neighbor.label]++;
        nearestNeighbors.pop();
    }

    // Find the label with the maximum count
    int maxCount = 0;
    std::string predictedLabel;
    for (const auto &labelCount: labelCounts) {
        if (labelCount.second > maxCount) {
            maxCount = labelCount.second;
            predictedLabel = labelCount.first;
        }
    }

    // Perform classification based on majority voting
    // For binary classification (habitable or not), assuming 1 represents 'habitable' and 0 represents 'not habitable'
    return (predictedLabel == "Habitable") ? 1 : 0;

}

void KNN::kNearestNeighborsUtil(KDTreeNode* node, const Point& queryPoint,
                                std::priority_queue<std::pair<double, Point>>& neighbors) {
    if (node == nullptr) {
        return;
    }

    if (node->isLeaf()) {
        // Calculate distance from queryPoint to each point in the leaf node's data bucket
        kd_tree_leaf_node* leafNode = dynamic_cast<kd_tree_leaf_node*>(node);
        for (const Point& dataPoint : leafNode->data.points) {
            double distance = calculateDistance(dataPoint.features, queryPoint.features);
            neighbors.push(std::make_pair(distance, dataPoint));

            if (neighbors.size() > k) {
                neighbors.pop(); // Keep only 'k' nearest neighbors
            }
        }
        return;
    }

    kd_tree_inter_node* interNode = dynamic_cast<kd_tree_inter_node*>(node);
    int axis = interNode->split_dimension;
    double splitValue = interNode->split_value;

    double distance = pow(queryPoint.features[axis] - splitValue, 2);
    bool exploreLeft = queryPoint.features[axis] <= splitValue;

    kNearestNeighborsUtil(exploreLeft ? interNode->left : interNode->right, queryPoint, neighbors);

    if (neighbors.size() < k || distance < neighbors.top().first) {
        kNearestNeighborsUtil(exploreLeft ? interNode->right : interNode->left, queryPoint, neighbors);
    }
}