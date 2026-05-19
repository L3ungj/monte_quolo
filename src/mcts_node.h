#pragma once

#include "game.h"

class MCTSNode {
public:
    GameState state;
    MCTSNode *parent;
    vector<Move> unexpandedMoves;
    vector<MCTSNode *> children;
    int wins, visits;
    MCTSNode(GameState state, MCTSNode *parent) : state(state), parent(parent), wins(0), visits(0) {
        unexpandedMoves = state.getLegalMoves(true);
    }
};