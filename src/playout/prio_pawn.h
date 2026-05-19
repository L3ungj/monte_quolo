#include "../policies.h"
#include <random>

class PrioPawn : public PlayoutPolicy {
    // Prioritise forward pawn moves
private:
    mt19937 gen;

public:
    PrioPawn() { gen.seed(random_device()()); }

    Move selectMove(const GameState &state) override {
        auto dist = state.getDistToGoal(state.p1Turn);
        Move bestPawnMove;
        int bestDist = 1000000;
        for (auto move : state.getLegalPawnMoves()) {
            if (dist[move.r][move.c] < bestDist) {
                bestDist = dist[move.r][move.c];
                bestPawnMove = move;
            }
        }

        // play best pawn move with 75% probability, random legal move otherwise
        uniform_int_distribution<> dis(1, 4);
        if(dis(gen) <= 3)
            return bestPawnMove;
        else {
            vector<Move> legalMoves = state.getLegalMoves(false);
            while (!legalMoves.empty()) {
                uniform_int_distribution<> dis(0, legalMoves.size() - 1);
                int idx = dis(gen);
                Move move = legalMoves[idx];
                if (state.isLegal(move, true))
                    return move;
                legalMoves.erase(legalMoves.begin() + idx);
            }
            state.print();
            throw runtime_error("PANIC: No legal move found");
        }
    }
};