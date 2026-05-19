#include "game.h"
#include "mcts_node.h"
#include "policies.h"
#include <memory>

class MCTSAgent {
public:
    MCTSNode *root;
    int iterations;

    unique_ptr<SelectionPolicy> selectionPolicy;
    unique_ptr<ExpansionPolicy> expansionPolicy;
    unique_ptr<PlayoutPolicy> playoutPolicy;

    MCTSAgent(const GameState &initialState, unique_ptr<SelectionPolicy> selectionPolicy,
              unique_ptr<ExpansionPolicy> expansionPolicy, unique_ptr<PlayoutPolicy> playoutPolicy);

    MCTSNode *select();
    MCTSNode *expand(MCTSNode *node);
    bool playout(MCTSNode *node);
    void backpropogate(MCTSNode *node, bool win);

    Move getMove(int msLimit);
    double getWinRate(MCTSNode *node) { return node->visits > 0 ? (double)node->wins / node->visits : 0; }
};