#include "../policies.h"
#include <cmath>

class UCT : public SelectionPolicy {
public:
    double c;
    UCT(double c=1.414) : c(c) {}

    MCTSNode *selectChild(MCTSNode *node) override {
        MCTSNode *bestChild = nullptr;
        double bestValue = -1.0;
        for (MCTSNode *child : node->children) {
            double winRate = static_cast<double>(child->wins) / child->visits;
            double uctValue = winRate + c * sqrt(log(node->visits) / child->visits);
            if (uctValue > bestValue) {
                bestValue = uctValue;
                bestChild = child;
            }
        }
        return bestChild;
    }
};