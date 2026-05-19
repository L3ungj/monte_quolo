#pragma once

#include "mcts_node.h"

class SelectionPolicy {
public:
    virtual MCTSNode *selectChild(MCTSNode *node) = 0;
};

class ExpansionPolicy {
public:
    virtual Move expandMove(vector<Move> &moves) = 0;
};

class PlayoutPolicy {
public:
    virtual Move selectMove(const GameState &state) = 0;
};
