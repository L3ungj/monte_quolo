# Monte Quolo

Monte Quolo is an implementation of the board game Quoridor and the Monte Carlo Tree Search (MCTS) algorithm. Its purpose is to improve my understanding of MCTS and I have plans to extend this AI into a stronger agent like AlphaZero using self-play and deep learning.

# Code structure
The code in `src/` is organised into several classes:
- `GameState`: Represents the current state of the game, and implements the rules of Quoridor.
- `MCTSAgent`: Implements the Monte Carlo Tree Search algorithm to select moves.
- `MCTSNode`: Represents a node in the MCTS tree, storing statistics and child nodes.
- `*Policy`: An abstract class for different policies used in selection, expansion, and playout phases of MCTS.
- Concrete policies can be found in directories like `selection/`, `expand/`, and `playout/`.

# Usage
To compile the code, run `g++ -O2 -g src/*.cpp -o quo`.

Upon running the program, you will be prompted to enter moves. You can:
- Enter a move in algebraic notation, e.g. `e2`, `hc3`, `vh6`.
- Enter `genmove <timeLimit>` to have the MCTS agent generate a move within the specified time limit in seconds.

# PrioPawn heuristic
The PrioPawn heuristic is a playout policy that prioritises moving the pawn towards the goal. In 75% of the time, it will select a move that brings the pawn closer to the goal, and in 25% of the time, it will select a random legal move. This makes playouts to terminate faster.

This heuristic is inspired by [Quoridor AI based on Monte Carlo Tree Search](https://github.com/gorisanson/quoridor-ai)

# barricade.gg puzzles
The website barricade.gg has three puzzles every day. The agent with the PrioPawn heuristic can solve the first two under 1 second usually, but struggles with the third puzzle which is more complex (usually needs 10-600s). To input puzzles, paste the HTML code of the move history into `initial_moves.html` and run the program with `-i`.

# Acknowledgements
- This project is inspired by [Quoridor AI based on Monte Carlo Tree Search](https://github.com/gorisanson/quoridor-ai)
- [Barricade Engine](https://github.com/Q-X-F/barricade-engine) is a minimax AI for Quoridor created by my friend that I used to test the strength of my MCTS agent.