#include "../policies.h"
#include <random>
#include <algorithm>

class RandomExpand : public ExpansionPolicy {
private:
    mt19937 gen;

public:
    RandomExpand() { gen.seed(random_device()()); }

    Move expandMove(vector<Move> &moves) override {
        uniform_int_distribution<> dis(0, moves.size() - 1);
        int index = dis(gen);
        Move move = moves[index];
        moves.erase(moves.begin() + index);
        return move;
    }
};