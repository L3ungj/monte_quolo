#include "../policies.h"
#include <random>

class RandomPlayout : public PlayoutPolicy {
private:
    mt19937 gen;

public:
    RandomPlayout() { gen.seed(random_device()()); }

    Move selectMove(const GameState &state) override {
        vector<Move> legalMoves = state.getLegalMoves(false);
        while (!legalMoves.empty()) {
            uniform_int_distribution<> dis(0, legalMoves.size() - 1);
            int idx = dis(gen);
            Move move = legalMoves[idx];
            if (state.isLegal(move, true))
                return move;
            legalMoves.erase(legalMoves.begin() + idx);
        }
        throw runtime_error("PANIC: No legal move found");
    }
};