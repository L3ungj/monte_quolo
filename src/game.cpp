#include "game.h"

Move::Move(MoveType type, char r, char c) : type(type), r(r), c(c) {}

Move::Move(string moveStr) {
    if (moveStr.length() == 3 && moveStr[0] == 'h') {
        type = MoveType::HORI;
        c = moveStr[1] - 'a';
        r = moveStr[2] - '1';
    } else if (moveStr.length() == 3 && moveStr[0] == 'v') {
        type = MoveType::VERT;
        c = moveStr[1] - 'a';
        r = moveStr[2] - '1';
    } else {
        type = MoveType::PAWN;
        c = moveStr[0] - 'a';
        r = moveStr[1] - '1';
    }
}

ostream &operator<<(ostream &os, const Move &move) {
    if (move.type == MoveType::PAWN) {
        os << static_cast<char>(move.c + 'a') << static_cast<char>(move.r + '1');
    } else if (move.type == MoveType::HORI) {
        os << "h" << static_cast<char>(move.c + 'a')
           << static_cast<char>(move.r + '1');
    } else if (move.type == MoveType::VERT) {
        os << "v" << static_cast<char>(move.c + 'a')
           << static_cast<char>(move.r + '1');
    }
    return os;
}

GameState::GameState() {
    horiWalls.fill({false});
    vertWalls.fill({false});
    p1r = 0;
    p1c = 4;
    p2r = 8;
    p2c = 4;
    p1WallsLeft = 10;
    p2WallsLeft = 10;
}

GameState::GameState(const GameState &other) {
    horiWalls = other.horiWalls;
    vertWalls = other.vertWalls;
    p1r = other.p1r;
    p1c = other.p1c;
    p2r = other.p2r;
    p2c = other.p2c;
    p1WallsLeft = other.p1WallsLeft;
    p2WallsLeft = other.p2WallsLeft;
    p1Turn = other.p1Turn;
}

bool GameState::operator==(const GameState &other) const {
    return horiWalls == other.horiWalls && vertWalls == other.vertWalls &&
           p1r == other.p1r && p1c == other.p1c && p2r == other.p2r &&
           p2c == other.p2c && p1WallsLeft == other.p1WallsLeft &&
           p2WallsLeft == other.p2WallsLeft && p1Turn == other.p1Turn;
}

bool GameState::isHoriBlocked(int r, int c) const {
    // checks if there's a horizontal wall blocking movement between (r,c) and
    // (r+1,c)
    return (c < 8 && horiWalls[r][c]) || (c > 0 && horiWalls[r][c - 1]);
}

bool GameState::isVertBlocked(int r, int c) const {
    // checks if there's a vertical wall blocking movement between (r,c) and
    // (r,c+1)
    return (r < 8 && vertWalls[r][c]) || (r > 0 && vertWalls[r - 1][c]);
}

bool GameState::canMoveUp(int r, int c) const {
    return r < 8 && !isHoriBlocked(r, c);
}

bool GameState::canMoveDown(int r, int c) const {
    return r > 0 && !isHoriBlocked(r - 1, c);
}

bool GameState::canMoveLeft(int r, int c) const {
    return c > 0 && !isVertBlocked(r, c - 1);
}

bool GameState::canMoveRight(int r, int c) const {
    return c < 8 && !isVertBlocked(r, c);
}

void GameState::print() const {
    cout << "============================================" << endl;
    for (int r = 8; r >= 0; r--) {
        cout << r + 1 << " ";
        for (int c = 0; c <= 8; c++) {
            if (p1r == r && p1c == c) {
                cout << "P1";
            } else if (p2r == r && p2c == c) {
                cout << "P2";
            } else {
                cout << "..";
            }
            if (c < 8) {
                cout << (isVertBlocked(r, c) ? " # " : "   ");
            }
        }
        cout << endl
             << "  ";
        if (r > 0) {
            for (int c = 0; c <= 8; c++) {
                cout << (isHoriBlocked(r - 1, c) ? "--" : "  ");
                if (c < 8) {
                    cout << (horiWalls[r - 1][c] ? "---" : vertWalls[r - 1][c] ? " # "
                                                                               : "   ");
                }
            }
            cout << endl;
        }
    }
    for (char c = 0; c <= 8; c++) {
        cout << static_cast<char>(c + 'a') << "    ";
    }
    cout << endl;
    cout << "P1: " << (int)p1WallsLeft << "               " << (p1WallsLeft < 10 ? " " : "")
        << (p1Turn ? "P1" : "P2") << "               "
        << (p2WallsLeft < 10 ? " " : "") << "P2: " << (int)p2WallsLeft << endl;
    cout << "============================================" << endl;
}

bool GameState::isTerminal() const { return p1r == 8 || p2r == 0; }

bool GameState::p1Won() const { return p1r == 8; }
bool GameState::p2Won() const { return p2r == 0; }

array<array<int, 9>, 9> GameState::getDistToGoal(bool isP1) const {
    array<array<int, 9>, 9> dist;
    for (auto &row : dist)
        row.fill(-1);
    queue<pair<int, int>> q;
    // multi source bfs
    for (int c = 0; c <= 8; c++) {
        int r = isP1 ? 8 : 0;
        dist[r][c] = 0;
        q.push({r, c});
    }
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        int d = dist[r][c];
        if (canMoveUp(r, c) && dist[r + 1][c] == -1) {
            dist[r + 1][c] = d + 1;
            q.push({r + 1, c});
        }
        if (canMoveDown(r, c) && dist[r - 1][c] == -1) {
            dist[r - 1][c] = d + 1;
            q.push({r - 1, c});
        }
        if (canMoveLeft(r, c) && dist[r][c - 1] == -1) {
            dist[r][c - 1] = d + 1;
            q.push({r, c - 1});
        }
        if (canMoveRight(r, c) && dist[r][c + 1] == -1) {
            dist[r][c + 1] = d + 1;
            q.push({r, c + 1});
        }
    }
    return dist;
}

int GameState::p1Dist() const {
    return getDistToGoal(true)[p1r][p1c];
}

int GameState::p2Dist() const {
    return getDistToGoal(false)[p2r][p2c];
}

bool GameState::isLegal(Move move, bool connCheck) const {
    if (move.type == MoveType::PAWN) {
        if (move.r == p1r && move.c == p1c)
            return false;
        if (move.r == p2r && move.c == p2c)
            return false;

        if (move.r < 0 || move.r > 8 || move.c < 0 || move.c > 8)
            return false;

        int r = p1Turn ? p1r : p2r;
        int c = p1Turn ? p1c : p2c;
        int dr = move.r - (p1Turn ? p1r : p2r);
        int dc = move.c - (p1Turn ? p1c : p2c);

        if (dr == 1 && dc == 0)
            return canMoveUp(r, c);
        if (dr == -1 && dc == 0)
            return canMoveDown(r, c);

        if (dr == 0 && dc == 1)
            return canMoveRight(r, c);
        if (dr == 0 && dc == -1)
            return canMoveLeft(r, c);

        int dr2 = (p1Turn ? p2r - p1r : p1r - p2r);
        int dc2 = (p1Turn ? p2c - p1c : p1c - p2c);
        if (dr2 == 1 && dc2 == 0) {
            if (!canMoveUp(r, c))
                return false;
            if (r + 2 <= 8 && canMoveUp(r + 1, c)) {
                return dr == 2 && dc == 0;
            }
            if (dr == 1 && dc == 1 && canMoveRight(r + 1, c))
                return true;
            if (dr == 1 && dc == -1 && canMoveLeft(r + 1, c))
                return true;
        } else if (dr2 == -1 && dc2 == 0) {
            if (!canMoveDown(r, c))
                return false;
            if (r - 2 >= 0 && canMoveDown(r - 1, c)) {
                return dr == -2 && dc == 0;
            }
            if (dr == -1 && dc == 1 && canMoveRight(r - 1, c))
                return true;
            if (dr == -1 && dc == -1 && canMoveLeft(r - 1, c))
                return true;
        } else if (dr2 == 0 && dc2 == 1) {
            if (!canMoveRight(r, c))
                return false;
            if (c + 2 <= 8 && canMoveRight(r, c + 1)) {
                return dr == 0 && dc == 2;
            }
            if (dc == 1 && dr == 1 && canMoveUp(r, c + 1))
                return true;
            if (dc == 1 && dr == -1 && canMoveDown(r, c + 1))
                return true;
        } else if (dr2 == 0 && dc2 == -1) {
            if (!canMoveLeft(r, c))
                return false;
            if (c - 2 >= 0 && canMoveLeft(r, c - 1)) {
                return dr == 0 && dc == -2;
            }
            if (dc == -1 && dr == 1 && canMoveUp(r, c - 1))
                return true;
            if (dc == -1 && dr == -1 && canMoveDown(r, c - 1))
                return true;
        }
        return false;
    } else if (move.type == MoveType::HORI) {
        if (p1Turn && p1WallsLeft == 0)
            return false;
        if (!p1Turn && p2WallsLeft == 0)
            return false;
        if (move.r < 0 || move.r >= 8 || move.c < 0 || move.c >= 8)
            return false;
        if (isHoriBlocked(move.r, move.c) || isHoriBlocked(move.r, move.c + 1) ||
            vertWalls[move.r][move.c])
            return false;
        if (!connCheck)
            return true;
        const_cast<array<array<bool, 8>, 8> &>(horiWalls)[move.r][move.c] = true;
        bool p1CanReachGoal = p1Dist() != -1;
        bool p2CanReachGoal = p2Dist() != -1;
        const_cast<array<array<bool, 8>, 8> &>(horiWalls)[move.r][move.c] = false;
        if (!p1CanReachGoal || !p2CanReachGoal)
            return false;
    } else if (move.type == MoveType::VERT) {
        if (p1Turn && p1WallsLeft == 0)
            return false;
        if (!p1Turn && p2WallsLeft == 0)
            return false;
        if (move.r < 0 || move.r >= 8 || move.c < 0 || move.c >= 8)
            return false;
        if (isVertBlocked(move.r, move.c) || isVertBlocked(move.r + 1, move.c) ||
            horiWalls[move.r][move.c])
            return false;
        if (!connCheck)
            return true;
        const_cast<array<array<bool, 8>, 8> &>(vertWalls)[move.r][move.c] = true;
        bool p1CanReachGoal = p1Dist() != -1;
        bool p2CanReachGoal = p2Dist() != -1;
        const_cast<array<array<bool, 8>, 8> &>(vertWalls)[move.r][move.c] = false;
        if (!p1CanReachGoal || !p2CanReachGoal)
            return false;
    }
    return true;
}

bool GameState::makeMove(Move move, bool legalCheck) {
    if (legalCheck && !isLegal(move, true))
        return false;
    if (move.type == MoveType::PAWN) {
        if (p1Turn) {
            p1r = move.r;
            p1c = move.c;
        } else {
            p2r = move.r;
            p2c = move.c;
        }
    } else if (move.type == MoveType::HORI) {
        horiWalls[move.r][move.c] = true;
        if (p1Turn)
            p1WallsLeft--;
        else
            p2WallsLeft--;
    } else if (move.type == MoveType::VERT) {
        vertWalls[move.r][move.c] = true;
        if (p1Turn)
            p1WallsLeft--;
        else
            p2WallsLeft--;
    }
    p1Turn = !p1Turn;
    return true;
}

vector<Move> GameState::getLegalMoves(bool connCheck) const {
    vector<Move> moves = getLegalPawnMoves();
    vector<Move> wallMoves = getLegalWallMoves(connCheck);
    moves.insert(moves.end(), wallMoves.begin(), wallMoves.end());
    return moves;
}

const array<pair<int, int>, 4> DIRS = {{{1, 0}, {-1, 0}, {0, -1}, {0, 1}}};

vector<Move> GameState::getLegalPawnMoves() const {
    vector<Move> moves;
    int r = p1Turn ? p1r : p2r;
    int c = p1Turn ? p1c : p2c;
    int oppr = p1Turn ? p2r : p1r;
    int oppc = p1Turn ? p2c : p1c;
    for (auto &[dr, dc] : DIRS) {
        Move move(MoveType::PAWN, r + dr, c + dc);
        if (isLegal(move, true)) {
            moves.push_back(move);
        }
        Move jumpMove(MoveType::PAWN, oppr + dr, oppc + dc);
        if (isLegal(jumpMove, true)) {
            moves.push_back(jumpMove);
        }
    }
    return moves;
}

vector<Move> GameState::getLegalWallMoves(bool connCheck) const {
    if (p1Turn && p1WallsLeft == 0)
        return {};
    if (!p1Turn && p2WallsLeft == 0)
        return {};
    vector<Move> moves;
    for (char r = 0; r < 8; r++) {
        for (char c = 0; c < 8; c++) {
            Move horiMove(MoveType::HORI, r, c);
            if (isLegal(horiMove, connCheck)) {
                moves.push_back(horiMove);
            }
            Move vertMove(MoveType::VERT, r, c);
            if (isLegal(vertMove, connCheck)) {
                moves.push_back(vertMove);
            }
        }
    }
    return moves;
}

Move GameState::deduceMove(const GameState &other) const {
    if (p1WallsLeft == other.p1WallsLeft && p2WallsLeft == other.p2WallsLeft) {
        return p1Turn ? Move(MoveType::PAWN, other.p1r, other.p1c)
                      : Move(MoveType::PAWN, other.p2r, other.p2c);
    } else {
        for (char r = 0; r < 8; r++) {
            for (char c = 0; c < 8; c++) {
                if (!horiWalls[r][c] && other.horiWalls[r][c])
                    return Move(MoveType::HORI, r, c);
                if (!vertWalls[r][c] && other.vertWalls[r][c])
                    return Move(MoveType::VERT, r, c);
            }
        }
    }
    throw runtime_error("PANIC deduceMove: No move can reach the given state");
}