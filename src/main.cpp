#include <iostream>
#include <memory>
#include <fstream>

#include "game.h"
#include "mcts_agent.h"
#include "policies.h"

#include "selection/UCT.h"
#include "expand/random_expand.h"

#include "playout/random_playout.h"
#include "playout/prio_pawn.h"

int main(int argc, char *argv[]) {
    GameState game;
    if(argc == 2 && string(argv[1]) == "-i") {
        system("py extract_moves.py");
        ifstream initMovesFile("initial_moves.txt");
        string moveStr;
        while (initMovesFile >> moveStr) {
            Move move(moveStr);
            game.makeMove(move, true);
        }
    }
    while (!game.isTerminal()) {
        game.print();
        string moveStr;
        cout << "Enter move: ";
        cin >> moveStr;
        if(moveStr == "genmove") {
            MCTSAgent agent(game, make_unique<UCT>(), make_unique<RandomExpand>(), make_unique<PrioPawn>());
            int timeLimit;
            cin >> timeLimit;
            Move mctsMove = agent.getMove(timeLimit * 1000);
            if (game.makeMove(mctsMove, true)) {
                cout << "MCTS move: " << mctsMove << endl;
                cout << "Nodes explored: " << agent.iterations << endl;
                cout << "Win rate of opponent: " << agent.getWinRate(agent.root) << endl;
            } else {
                cout << "MCTS selected illegal move: " << mctsMove << endl;
            }
            continue;
        }
        Move move(moveStr);
        if (game.makeMove(move, true)) {
            cout << "Move made: " << move << endl;
        } else {
            cout << "Illegal move: " << move << endl;
        }
    }
}