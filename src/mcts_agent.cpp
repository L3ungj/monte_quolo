#include "mcts_agent.h"

#include <chrono>

MCTSAgent::MCTSAgent(const GameState &initialState, unique_ptr<SelectionPolicy> selectionPolicy,
                     unique_ptr<ExpansionPolicy> expansionPolicy, unique_ptr<PlayoutPolicy> playoutPolicy)
    : selectionPolicy(move(selectionPolicy)), expansionPolicy(move(expansionPolicy)),
      playoutPolicy(move(playoutPolicy)), iterations(0) {
    root = new MCTSNode(initialState, nullptr);
}

MCTSNode *MCTSAgent::select() {
    MCTSNode *node = root;
    while (node->unexpandedMoves.empty()) {
        node = selectionPolicy->selectChild(node);
    }
    return node;
}

MCTSNode *MCTSAgent::expand(MCTSNode *node) {
    Move move = expansionPolicy->expandMove(node->unexpandedMoves);
    GameState childState = node->state;
    childState.makeMove(move, false);
    MCTSNode *childNode = new MCTSNode(childState, node);
    node->children.push_back(childNode);
    return childNode;
}

bool MCTSAgent::playout(MCTSNode *node) {
    GameState state = node->state;
    int rounds = 0;
    while (!state.isTerminal()) {
        Move move = playoutPolicy->selectMove(state);
        state.makeMove(move, false);
        if(++rounds % 200 == 0) {
            state.print();
            cout << "WARN: Round " << rounds << endl;
        }
    }
    bool win = node->state.p1Turn ? state.p2Won() : state.p1Won();
    return win;
}

void MCTSAgent::backpropogate(MCTSNode *node, bool win) {
    while (node != nullptr) {
        node->visits++;
        if (win)
            node->wins++;
        win = !win;
        node = node->parent;
    }
}

Move MCTSAgent::getMove(int msLimit) {
    auto startTime = chrono::steady_clock::now();
    iterations = 0;
    while (chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - startTime).count() < msLimit) {
        MCTSNode *node = select();
        MCTSNode *expandedNode = expand(node);
        bool win = playout(expandedNode);
        backpropogate(expandedNode, win);
        ++iterations;
    }
    MCTSNode *bestChild = nullptr;
    int maxVisits = -1;
    for (MCTSNode *child : root->children) {
        if (child->visits > maxVisits) {
            maxVisits = child->visits;
            bestChild = child;
        }
        // cout << "Move: " << root->state.deduceMove(child->state) << ", Win rate: " << getWinRate(child) << ", Visits: " << child->visits << endl;
    }
    return root->state.deduceMove(bestChild->state);
}